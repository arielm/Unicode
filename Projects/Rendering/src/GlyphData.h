/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * WARNING:
 * IF THE GlyphData IS NOT FOR "IMMEDIATE CONSUMPTION", INVOKE copyDataAndReleaseSlot()
 * OTHERWISE, THE DATA WILL BECOME CORRUPTED UPON THE NEXT FT_Get_Glyph() OPERATION
 */

#pragma once

#include "FreetypeHelper.h"

#include "cinder/Vector.h"

namespace chronotext
{
    namespace zf
    {
        class GlyphData
        {
        public:
            bool useMipmap;
            int padding;
            
            int width;
            int height;
            ci::Vec2f offset;
            ci::Vec2f size;
            
            GlyphData(FT_Face ftFace, uint32_t codepoint, bool useMipmap, int padding)
            :
            useMipmap(useMipmap),
            padding(padding),
            ftGlyph(NULL),
            data(NULL)
            {
                if (codepoint > 0)
                {
                    if (!FT_Load_Glyph(ftFace, codepoint, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT))
                    {
                        ftSlot = ftFace->glyph;
                        
                        if (!FT_Get_Glyph(ftSlot, &ftGlyph))
                        {
                            FT_Render_Glyph(ftSlot, FT_RENDER_MODE_NORMAL);
                            
                            width = ftSlot->bitmap.width;
                            height = ftSlot->bitmap.rows;
                            
                            if (width * height > 0)
                            {
                                offset = ci::Vec2f(ftSlot->bitmap_left, -ftSlot->bitmap_top) - ci::Vec2f(padding, padding);
                                size = ci::Vec2f(width, height) + ci::Vec2f(padding, padding) * 2;
                            }
                            else
                            {
                                FT_Done_Glyph(ftGlyph);
                                ftGlyph = NULL;
                            }
                        }
                    }
                }
            }
            
            ~GlyphData()
            {
                if (ftGlyph)
                {
                    FT_Done_Glyph(ftGlyph);
                }
                else if (data)
                {
                    free(data);
                }
            }
            
            bool isValid() const
            {
                return (ftGlyph || data);
            }
            
            unsigned char* getBuffer() const
            {
                if (ftGlyph)
                {
                    return ftSlot->bitmap.buffer;
                }
                else if (data)
                {
                    return data;
                }
                else
                {
                    return NULL;
                }
            }
            
            void copyDataAndReleaseSlot()
            {
                if (ftGlyph)
                {
                    int dataSize = width * height;
                    data = (unsigned char*)malloc(dataSize);
                    memcpy(data, ftSlot->bitmap.buffer, dataSize);
                    
                    FT_Done_Glyph(ftGlyph);
                    ftGlyph = NULL;
                }
            }
            
        protected:
            FT_GlyphSlot ftSlot;
            FT_Glyph ftGlyph;
            unsigned char *data;
        };
    }
}

namespace chr = chronotext;
