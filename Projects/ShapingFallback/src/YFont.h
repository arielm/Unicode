/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "FreetypeHelper.h"
#include "TextSpan.h"

#include "cinder/gl/Texture.h"

class YFont;

struct FontDescriptor
{
    ci::DataSourceRef source;
    int faceIndex;
    
    FontDescriptor(ci::DataSourceRef source, int faceIndex = 0)
    :
    source(source),
    faceIndex(faceIndex)
    {}
};

class Shape
{
public:
    YFont *font;
    uint32_t cluster;
    hb_codepoint_t codepoint;
    ci::Vec2f offset;
    float advance;
    
    Shape(uint32_t cluster);
    Shape(YFont *font, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
    
    void update(YFont *font, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
    float draw();
};

class YGlyph
{
public:
    ci::gl::TextureRef texture;
    ci::Vec2f offset;
    
    YGlyph(unsigned char *data, int width, int height);
};

class YFont
{
public:
    ci::Vec2f scale;
    
    float leading;
    float ascent;
    float descent;
    
    hb_font_t *hbFont;
    
    YFont(std::shared_ptr<FreetypeHelper> ftHelper, const FontDescriptor &descriptor, float size);
    ~YFont();
    
    YGlyph* createGlyph(uint32_t codepoint) const;
    
protected:
    std::shared_ptr<FreetypeHelper> ftHelper;
    FT_Face face;
};
