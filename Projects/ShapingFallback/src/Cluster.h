#pragma once

#include "YFont.h"

struct Shape
{
    hb_codepoint_t codepoint;
    ci::Vec2f position;
    float advance;
    
    Shape(uint32_t codepoint, const ci::Vec2f &position, float advance)
    :
    codepoint(codepoint),
    position(position),
    advance(advance)
    {}
};

class Cluster
{
public:
    YFont *font;
    float combinedAdvance;
    std::vector<Shape> shapes;
    
    Cluster(YFont *font, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance)
    :
    font(font),
    combinedAdvance(advance)
    {
        shapes.emplace_back(codepoint, offset, advance);
    }
    
    void addShape(hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance)
    {
        shapes.emplace_back(codepoint, ci::Vec2f(combinedAdvance, 0) + offset, advance);
        combinedAdvance += advance;
    }
    
    float draw()
    {
        for (auto shape : shapes)
        {
            YGlyph *glyph = font->getGlyph(shape.codepoint);
            
            if (glyph && glyph->texture)
            {
                ci::gl::color(font->color);
                ci::gl::draw(glyph->texture, shape.position + glyph->offset);
            }
        }
        
        return combinedAdvance;
    }
};
