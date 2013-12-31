/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
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
    
    GlyphData(FT_GlyphSlot slot, bool useMipmap, int padding)
    :
    slot(slot),
    useMipmap(useMipmap),
    padding(padding),
    ftGlyph(NULL),
    data(NULL)
    {
        if (!FT_Get_Glyph(slot, &ftGlyph))
        {
            FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
            
            width = slot->bitmap.width;
            height = slot->bitmap.rows;
            
            if (width * height > 0)
            {
                offset = ci::Vec2f(slot->bitmap_left, -slot->bitmap_top) - ci::Vec2f(padding, padding);
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
    
    bool isValid()
    {
        return (ftGlyph || data);
    }
    
    unsigned char* getData()
    {
        if (ftGlyph)
        {
            return slot->bitmap.buffer;
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
            memcpy(data, slot->bitmap.buffer, dataSize);
            
            FT_Done_Glyph(ftGlyph);
            ftGlyph = NULL;
        }
    }
    
protected:
    FT_GlyphSlot slot;
    FT_Glyph ftGlyph;
    unsigned char *data;
};
