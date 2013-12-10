/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "YFont.h"

#include "hb-ft.h"

#include "cinder/Utilities.h"

using namespace std;
using namespace ci;

/*
 See http://www.microsoft.com/typography/otspec/name.htm for a list of some
 possible platform-encoding pairs.  We're interested in 0-3 aka 3-1 - UCS-2.
 Otherwise, fail. If a font has some unicode map, but lacks UCS-2 - it is a
 broken or irrelevant font. What exactly Freetype will select on face load
 (it promises most wide unicode, and if that will be slower that UCS-2 -
 left as an excercise to check.)
 */
static FT_Error force_ucs2_charmap(FT_Face face)
{
    for (int i = 0; i < face->num_charmaps; i++)
    {
        auto platform_id = face->charmaps[i]->platform_id;
        auto encoding_id = face->charmaps[i]->encoding_id;
        
        if (((platform_id == 0) && (encoding_id == 3)) || ((platform_id == 3) && (encoding_id == 1)))
        {
            return FT_Set_Charmap(face, face->charmaps[i]);
        }
    }
    
    return -1;
}

static int nextPowerOfTwo(int x)
{
    int result = 1;
    
    while (result < x)
    {
        result <<= 1;
    }
    
    return result;
}

YGlyph::YGlyph(unsigned char *data, int width, int height)
:
width(width),
height(height)
{
    if (width * height > 0)
    {
        gl::Texture::Format format;
        format.setInternalFormat(GL_ALPHA);
        format.enableMipmapping(true);
        format.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
        
        texture = gl::Texture::create(data, GL_ALPHA, width, height, format);
    }
}

YFont::YFont(shared_ptr<FreetypeHelper> ftHelper, const fs::path &filePath, float size)
:
ftHelper(ftHelper)
{
    FT_Error error = FT_New_Face(ftHelper->getLib(), filePath.c_str(), 0, &face);
    
    if (error)
    {
        throw runtime_error("FREETYPE: ERROR " + toString(error));
    }
    
    if (force_ucs2_charmap(face))
    {
        throw runtime_error("HARFBUZZ: FONT IS BROKEN OR IRRELEVANT");
    }
    
    // ---
    
    /*
     * USING A MATRIX WITH A MULTIPLIER ALLOWS FOR FRACTIONAL VALUES
     * TRICK FROM http://code.google.com/p/freetype-gl/
     *
     * - WITHOUT A FRACTIONAL ADVANCE: CHARACTER SPACING LOOKS DUMB
     * - WITHOUT A FRACTIONAL HEIGHT: SOME CHARACTERS WON'T BE PROPERLY ALIGNED ON THE BASELINE
     */
    int res = 64;
    int dpi = 72;
    
    scale = Vec2f::one() / Vec2f(res, res) / 64;
    FT_Set_Char_Size(face, size * 64, 0, dpi * res, dpi * res);
    
    FT_Matrix matrix =
    {
        int((1.0 / res) * 0x10000L),
        int((0.0) * 0x10000L),
        int((0.0) * 0x10000L),
        int((1.0 / res) * 0x10000L)
    };
    
    FT_Set_Transform(face, &matrix, NULL);
    
    leading = face->size->metrics.height * scale.y;
    ascent = face->size->metrics.ascender * scale.y;
    descent = -face->size->metrics.descender * scale.y;
    
    // ---
    
    hbFont = hb_ft_font_create(face, NULL);
    hbBuffer = hb_buffer_create();
}

YFont::~YFont()
{
    clearCache();
    
    hb_buffer_destroy(hbBuffer);
    hb_font_destroy(hbFont);
    
    FT_Done_Face(face);
}

LineLayout YFont::createLayout(const TextSpan &span) const
{
    hb_buffer_reset(hbBuffer);
    
    hb_buffer_set_script(hbBuffer, span.script);
    hb_buffer_set_direction(hbBuffer, span.direction);
    
    size_t textSize = span.text.size();
    hb_buffer_add_utf8(hbBuffer, span.text.data(), textSize, 0, textSize);
    hb_shape(hbFont, hbBuffer, NULL, 0);
    
    unsigned int glyphCount;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(hbBuffer, &glyphCount);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(hbBuffer, &glyphCount);
    
    // ---

    Vec2f p;
    LineLayout layout;
    
    for (int i = 0; i < glyphCount; i++)
    {
        const hb_glyph_position_t &pos = glyph_pos[i];
        Vec2f offset(pos.x_offset, -pos.y_offset);
        Vec2f advance(pos.x_advance, pos.y_advance);
        
        layout.push_back(make_pair(glyph_info[i].codepoint, p + offset * scale));
        p += advance * scale;
    }
    
    return layout;
}

void YFont::drawLayout(const LineLayout &layout, const Vec2f &origin)
{
    for (auto entry : layout)
    {
        YGlyph *glyph = getGlyph(entry.first);
        
        if (glyph && glyph->texture)
        {
            gl::draw(glyph->texture, origin + entry.second + glyph->offset);
        }
    }
}

string YFont::getName() const
{
    return string(face->family_name) + " " + string(face->style_name);
}

YGlyph* YFont::getGlyph(uint32_t codepoint)
{
    auto entry = cache.find(codepoint);
    
    if (entry == cache.end())
    {
        YGlyph *glyph = createGlyph(codepoint);
        
        if (glyph)
        {
            cache[codepoint] = glyph;
        }
        
        return glyph;
    }
    else
    {
        return entry->second;
    }
}

YGlyph* YFont::createGlyph(uint32_t codepoint) const
{
    if (codepoint > 0)
    {
        FT_Error error = FT_Load_Glyph(face, codepoint, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT);
        
        if (!error)
        {
            FT_GlyphSlot slot = face->glyph;
            
            FT_Glyph glyph;
            error = FT_Get_Glyph(slot, &glyph);
            
            if (!error)
            {
                FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
                
                YGlyph *g = new YGlyph(slot->bitmap.buffer, slot->bitmap.width, slot->bitmap.rows);
                g->offset = Vec2f(slot->bitmap_left, -slot->bitmap_top);
                
                FT_Done_Glyph(glyph);
                return g;
            }
        }
    }
    
    return NULL;
}

void YFont::clearCache()
{
    for (auto entry : cache)
    {
        delete entry.second;
    }
    
    cache.clear();
}
