#pragma once

#include "YFont.h"

class Cluster
{
public:
    YFont *font;
    float combinedAdvance;
    std::vector<std::pair<hb_codepoint_t, ci::Vec2f>> shapes;
    
    Cluster(YFont *font, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance)
    :
    font(font),
    combinedAdvance(advance)
    {
        shapes.emplace_back(codepoint, offset);
    }
    
    void addShape(hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance)
    {
        shapes.emplace_back(codepoint, offset);
        combinedAdvance += advance;
    }
    
    float draw()
    {
        for (auto shape : shapes)
        {
            YGlyph *glyph = font->createGlyph(shape.first);
            
            if (glyph)
            {
                if (glyph->texture)
                {
                    ci::gl::color(font->color);
                    ci::gl::draw(glyph->texture, glyph->offset + shape.second);
                }
                
                delete glyph;
            }
        }
        
        return combinedAdvance;
    }
};
