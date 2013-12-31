/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * WARNING:
 * IF THE GlyphData IS NOT FOR "IMMEDIATE CONSUPTION", INVOKE copyDataAndReleaseSlot()
 * OTHERWISE, THE DATA WILL BE CORRUPTED UPON THE NEXT FT_Get_Glyph() OPERATION
 */

#pragma once

#include "FreetypeHelper.h"

#include "cinder/Vector.h"

class GlyphData
{
public:
    bool useMipmap;
    int padding;
    
    int width;
    int height;
    ci::Vec2f offset;
    ci::Vec2f size;
    
    GlyphData(FT_GlyphSlot ftSlot, bool useMipmap, int padding)
    :
    ftSlot(ftSlot),
    useMipmap(useMipmap),
    padding(padding),
    ftGlyph(NULL),
    data(NULL)
    {
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
        }
    }
    
    ~GlyphData()
    {
        if (ftGlyph)
        {
            std::cout << "GlyhData - RELEASE SLOT" << std::endl;
            FT_Done_Glyph(ftGlyph);
        }
        else if (data)
        {
            std::cout << "GlyhData - RELEASE DATA" << std::endl;
            free(data);
        }
    }
    
    bool isValid() const
    {
        return (ftGlyph || data);
    }
    
    unsigned char* getData() const
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
