/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "ActualFont.h"

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

ActualFont::ActualFont(shared_ptr<FreetypeHelper> ftHelper, const fs::path &filePath, float baseSize, bool useMipmap, int padding)
:
ftHelper(ftHelper),
baseSize(baseSize),
useMipmap(useMipmap),
padding(padding)
{
    FT_Error error = FT_New_Face(ftHelper->getLib(), filePath.c_str(), 0, &ftFace);
    
    if (error)
    {
        throw runtime_error("FREETYPE: ERROR " + toString(error));
    }
    
    if (force_ucs2_charmap(ftFace))
    {
        throw runtime_error("HARFBUZZ: FONT IS BROKEN OR IRRELEVANT");
    }
    
    // ---
    
    /*
     * USING A MATRIX WITH A MULTIPLIER ALLOWS FOR FRACTIONAL VALUES
     * TRICK FROM http://code.google.com/p/freetype-gl/
     *
     * - WITHOUT A FRACTIONAL ADVANCE: CHARACTER SPACING LOOKS DUMB
     * - WITHOUT A FRACTIONAL HEIGHT: SOME CHARACTERS WON'T BE PERFECTLY ALIGNED ON THE BASELINE
     */
    int res = 64;
    int dpi = 72;
    
    scale = Vec2f::one() / Vec2f(res, res) / 64;
    FT_Set_Char_Size(ftFace, baseSize * 64, 0, dpi * res, dpi * res);
    
    FT_Matrix matrix =
    {
        int((1.0 / res) * 0x10000L),
        int((0.0) * 0x10000L),
        int((0.0) * 0x10000L),
        int((1.0 / res) * 0x10000L)
    };
    
    FT_Set_Transform(ftFace, &matrix, NULL);
    
    // ---
    
    metrics.height = ftFace->size->metrics.height * scale.y;
    metrics.ascent = ftFace->size->metrics.ascender * scale.y;
    metrics.descent = -ftFace->size->metrics.descender * scale.y;

    metrics.lineThickness = ftFace->underline_thickness / 64.0f;
    metrics.underlineOffset = -ftFace->underline_position / 64.0f;
    
    //
    
    auto os2 = (TT_OS2*)FT_Get_Sfnt_Table(ftFace, ft_sfnt_os2);
    
    if (os2 && (os2->version != 0xFFFF))
    {
        metrics.strikethroughOffset = FT_MulFix(os2->yStrikeoutPosition, ftFace->size->metrics.y_scale) * scale.y;
    }
    else
    {
        metrics.strikethroughOffset = 0.5f * (metrics.ascent - metrics.descent);
    }
    
    // ---
    
    hbFont = hb_ft_font_create(ftFace, NULL);
}

ActualFont::~ActualFont()
{
    clearGlyphCache();
    
    hb_font_destroy(hbFont);
    FT_Done_Face(ftFace);
}

ActualFont::Glyph* ActualFont::getGlyph(uint32_t codepoint)
{
    auto entry = glyphCache.find(codepoint);
    
    if (entry == glyphCache.end())
    {
        Glyph *glyph = createGlyph(codepoint);
        
        if (glyph)
        {
            glyphCache[codepoint] = glyph;
        }
        
        return glyph;
    }
    else
    {
        return entry->second;
    }
}

void ActualFont::clearGlyphCache()
{
    for (auto entry : glyphCache)
    {
        delete entry.second;
    }
    
    glyphCache.clear();
}

ActualFont::Glyph* ActualFont::createGlyph(uint32_t codepoint)
{
    GlyphData glyphData(ftFace, codepoint, useMipmap, padding);
    
    if (glyphData.isValid())
    {
        auto texture = new ReloadableTexture(glyphData);
        standaloneTextures.push_back(unique_ptr<ReloadableTexture>(texture));
        
        return new Glyph(texture, glyphData.offset, glyphData.size);
    }
    else
    {
        return NULL;
    }
}
