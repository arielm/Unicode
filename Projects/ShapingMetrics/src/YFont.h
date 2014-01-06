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

struct ShapeLayout
{
    hb_direction_t direction;
    float advance;
    
    std::vector<Shape> shapes;
    
    ShapeLayout()
    {}
    
    ShapeLayout(hb_direction_t direction)
    :
    direction(direction),
    advance(0)
    {}
    
    void addShape(const Shape &shape)
    {
        shapes.push_back(shape);
        advance += shape.advance;
    }
};

class YGlyph
{
public:
    ci::gl::TextureRef texture;
    ci::Vec2f size;
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
    void drawLayout(const ShapeLayout &layout, ci::Vec2f origin, float scale);
    void drawMetrics(const ShapeLayout &layout, ci::Vec2f origin, float scale);
    std::string getName() const;

protected:
    std::shared_ptr<FreetypeHelper> ftHelper;
    FT_Face face;

    hb_font_t *hbFont;
    hb_buffer_t *hbBuffer;
    
    std::map<uint32_t, std::unique_ptr<YGlyph>> cache;

    YGlyph* getGlyph(uint32_t codepoint);
    YGlyph* createGlyph(uint32_t codepoint) const;
    void clearCache();
};
