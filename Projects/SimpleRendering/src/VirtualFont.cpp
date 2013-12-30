/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "VirtualFont.h"
#include "TextLayout.h"

using namespace std;
using namespace ci;

VirtualFont::VirtualFont(float baseSize)
:
baseSize(baseSize)
{
    setSize(1);
}

bool VirtualFont::add(const string &lang, ActualFont *font)
{
    if (font)
    {
        fontSetMap[lang].insert(font);
        return true;
    }
    
    return false;
}

FontSet VirtualFont::getFontSet(const string &lang) const
{
    auto it = fontSetMap.find(lang);
    
    if (it == fontSetMap.end())
    {
        it = fontSetMap.find("");
        
        if (it == fontSetMap.end())
        {
            return FontSet();
        }
    }
    
    return it->second;
}

ActualFont::Metrics VirtualFont::getMetrics(const string &lang) const
{
    auto it = fontSetMap.find(lang);
    
    if (it == fontSetMap.end())
    {
        it = fontSetMap.find("");
        
        if (it == fontSetMap.end())
        {
            return ActualFont::Metrics();
        }
    }
    
    return (*it->second.begin())->metrics;
}

void VirtualFont::setSize(float newSize)
{
    size = newSize;
    sizeRatio = newSize / baseSize;
}

float VirtualFont::getAdvance(const Cluster &cluster) const
{
    return cluster.combinedAdvance * sizeRatio;
}

float VirtualFont::getAdvance(const TextLayout &layout) const
{
    return layout.advance * sizeRatio;
}

void VirtualFont::drawCluster(const Cluster &cluster, const Vec2f &position)
{
    float sizeRatio = size / cluster.font->baseSize;
    
    for (auto shape : cluster.shapes)
    {
        auto glyph = cluster.font->getGlyph(shape.codepoint);
        
        if (glyph && glyph->texture)
        {
            auto corner = position + (shape.position + glyph->offset) * sizeRatio;
            gl::draw(*glyph->texture, Area(0, 0, glyph->size.x, glyph->size.y), Rectf(corner, corner + glyph->size * sizeRatio));
        }
    }
}
