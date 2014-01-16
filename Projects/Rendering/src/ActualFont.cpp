/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "ActualFont.h"

#include "chronotext/utils/Utils.h"

#include "hb-ft.h"

using namespace std;
using namespace ci;
using namespace chr;

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

ActualFont::ActualFont(shared_ptr<FreetypeHelper> ftHelper, InputSourceRef inputSource, float baseSize, bool useMipmap)
:
ftHelper(ftHelper),
inputSource(inputSource),
baseSize(baseSize),
useMipmap(useMipmap),
loaded(false),
ftFace(NULL),
hbFont(NULL)
{
    /*
     * PADDING IS NECESSARY TO AVOID BORDER ARTIFACTS
     *
     * MIPMAPPING IS EVEN ACCENTUATING THE PROBLEM
     * BECAUSE OF THE CONSECUTIVE TEXTURE SCALE-DOWNS
     */
    if (useMipmap)
    {
        padding = 2; // TODO: USE baseSize TO DEFINE AN OPTIMAL VALUE
    }
    else
    {
        padding = 1; // THE MINIMUM POSSIBLE
    }
    
    reload();
}

ActualFont::~ActualFont()
{
    unload();
}

void ActualFont::reload()
{
    if (!loaded)
    {
        if (!inputSource->isFile())
        {
            throw runtime_error("ActualFont MUST BE CREATED FROM FILE");
        }
        
        FT_Error error = FT_New_Face(ftHelper->getLib(), inputSource->getFilePath().c_str(), 0, &ftFace);
        
        if (error)
        {
            throw runtime_error("FREETYPE: ERROR " + toString(error));
        }
        
        if (force_ucs2_charmap(ftFace))
        {
            FT_Done_Face(ftFace);
            throw runtime_error("HARFBUZZ: FONT IS BROKEN OR IRRELEVANT");
        }
        
        hbFont = hb_ft_font_create(ftFace, NULL);

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
        
        loaded = true;
        LOGD << "LOADING FONT: " << ftFace->family_name << " "  << ftFace->style_name << endl;
    }
}

void ActualFont::unload()
{
    if (loaded)
    {
        loaded = false;
        LOGD << "UNLOADING FONT: " << ftFace->family_name << " "  << ftFace->style_name << endl;

        glyphCache.clear();
        standaloneTextures.clear();

        hb_font_destroy(hbFont); hbFont = NULL;
        FT_Done_Face(ftFace); ftFace = NULL;
    }
}

/*
 * THIS IS NOT DESTROYING THE ReloadableTexture INSTANCES
 * I.E. THE POINTER INSIDE ActualFont::Glyph REMAINS VALID
 */
void ActualFont::discardTextures()
{
    for (auto &texture : standaloneTextures)
    {
        texture->unload();
    }
}

ActualFont::Glyph* ActualFont::getGlyph(uint32_t codepoint)
{
    Glyph *glyph = NULL;
    
    if (loaded)
    {
        auto entry = glyphCache.find(codepoint);
        
        if (entry == glyphCache.end())
        {
            glyph = createGlyph(codepoint);
            
            if (glyph)
            {
                glyphCache[codepoint] = unique_ptr<Glyph>(glyph);
            }
            else
            {
                /*
                 * INSERTING A VALUE FOR TEXTURELESS GLYPHS (E.G. A "SPACE")
                 * IS NECESSARY, OTHERWISE createGlyph() WOULD BE INVOKED
                 * INDEFINITELY FOR THE SAME CODEPOINT
                 */
                glyphCache[codepoint] = unique_ptr<Glyph>(new Glyph());
            }
        }
        else
        {
            glyph = entry->second.get();
            
            /*
             * IN CASE A PREVIOUSLY-LOADED TEXTURE HAVE BEEN
             * DISCARDED, E.G. AFTER SOME OPENGL CONTEXT-LOSS
             */
            if (glyph->texture && !glyph->texture->isLoaded())
            {
                GlyphData glyphData(ftFace, codepoint, useMipmap, padding);
                
                if (glyphData.isValid())
                {
                    glyph->texture->load(glyphData);
                }
            }
        }
    }
    
    return glyph;
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
    
    return NULL;
}
