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

#include <map>

struct Shape
{
    uint32_t codepoint;
    ci::Vec2f position;
    float advance;
    
    Shape(uint32_t codepoint, const ci::Vec2f &position, float advance)
    :
    codepoint(codepoint),
    position(position),
    advance(advance)
    {}
};

typedef std::vector<Shape> ShapeLayout;

class YGlyph
{
public:
    int width;
    int height;
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
    
    YFont(std::shared_ptr<FreetypeHelper> ftHelper, const ci::fs::path &filePath, float size);
    ~YFont();
    
    ShapeLayout createLayout(const TextSpan &span);
    void drawLayout(const ShapeLayout &layout, const ci::Vec2f &origin);
    std::string getName() const;

protected:
    std::shared_ptr<FreetypeHelper> ftHelper;
    FT_Face face;

    hb_font_t *hbFont;
    hb_buffer_t *hbBuffer;
    
    std::map<uint32_t, YGlyph*> cache;

    YGlyph* getGlyph(uint32_t codepoint);
    YGlyph* createGlyph(uint32_t codepoint) const;
    void clearCache();
};
