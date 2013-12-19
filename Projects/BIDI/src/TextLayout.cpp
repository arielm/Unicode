#include "TextLayout.h"

using namespace std;
using namespace ci;

Cluster::Cluster(YFont *font, hb_codepoint_t codepoint, const Vec2f &offset, float advance)
:
font(font),
combinedAdvance(advance)
{
    shapes.emplace_back(codepoint, offset, advance);
}

void Cluster::addShape(hb_codepoint_t codepoint, const Vec2f &offset, float advance)
{
    shapes.emplace_back(codepoint, Vec2f(combinedAdvance, 0) + offset, advance);
    combinedAdvance += advance;
}

float Cluster::draw(const Vec2f &position)
{
    for (auto shape : shapes)
    {
        YGlyph *glyph = font->getGlyph(shape.codepoint);
        
        if (glyph && glyph->texture)
        {
            gl::color(font->color);
            gl::draw(glyph->texture, position + shape.position + glyph->offset);
        }
    }
    
    return combinedAdvance;
}

TextLayout::TextLayout()
:
advance(0)
{}

void TextLayout::addCluster(const Cluster &cluster)
{
    clusters.emplace_back(cluster, advance);
    advance += cluster.combinedAdvance;
}

void TextLayout::draw(const Vec2f &position)
{
    for (auto entry : clusters)
    {
        entry.first.draw(Vec2f(entry.second, 0) + position);
    }
}
