/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "VirtualFont.h"

using namespace std;
using namespace ci;

VirtualFont::VirtualFont(TextItemizer &itemizer, float baseSize)
:
itemizer(itemizer),
baseSize(baseSize)
{
    positions.reserve(4);
    coords.reserve(4);
    colors.reserve(4);
    
    setSize(baseSize);
}

bool VirtualFont::add(const string &lang, ActualFont *font)
{
    if (font)
    {
        /*
         * WHY USING std::vector INSTEAD OF std::set?
         * BECAUSE ORDER OF INSERTION MATTERS
         */
        
        for (auto tmp : fontSetMap[lang])
        {
            if (tmp == font)
            {
                return false;
            }
        }
        
        fontSetMap[lang].push_back(font);
        return true;
    }
    
    return false;
}

const FontSet& VirtualFont::getFontSet(const string &lang) const
{
    auto it = fontSetMap.find(lang);
    
    if (it == fontSetMap.end())
    {
        it = fontSetMap.find("");
        
        if (it == fontSetMap.end())
        {
            return defaultFontSet;
        }
    }
    
    return it->second;
}

ActualFont::Metrics VirtualFont::getRawMetrics(const string &lang) const
{
    auto &fontSet = getFontSet(lang);
    
    if (fontSet.empty())
    {
        return ActualFont::Metrics();
    }
    else
    {
        return fontSet.front()->metrics;
    }
}

ActualFont::Metrics VirtualFont::getMetrics(const string &lang) const
{
    return getRawMetrics(lang) * sizeRatio;
}

ActualFont::Metrics VirtualFont::getMetrics(const Cluster &cluster) const
{
    return cluster.font->metrics * sizeRatio;
}

float VirtualFont::getAscent(const LineLayout &layout) const
{
    return layout.maxAscent * sizeRatio;
}

float VirtualFont::getDescent(const LineLayout &layout) const
{
    return layout.maxDescent * sizeRatio;
}

float VirtualFont::getMiddleLine(const LineLayout &layout) const
{
    return 0.5f * (layout.maxAscent - layout.maxDescent) * sizeRatio;
}

float VirtualFont::getOffsetX(const LineLayout &layout, Alignment align) const
{
    switch (align)
    {
        case ALIGN_RIGHT:
            return -getAdvance(layout);
            
        case ALIGN_MIDDLE:
            return -0.5f * getAdvance(layout);
            
        default:
            return 0;
    }
}

float VirtualFont::getOffsetY(const LineLayout &layout, Alignment align) const
{
    switch (align)
    {
        case ALIGN_TOP:
            return +getAscent(layout);
            
        case ALIGN_MIDDLE:
            return getMiddleLine(layout);

        case ALIGN_BOTTOM:
            return -getDescent(layout);

        default:
            return 0;
    }
}

float VirtualFont::getAdvance(const Cluster &cluster) const
{
    return cluster.combinedAdvance * sizeRatio;
}

float VirtualFont::getAdvance(const LineLayout &layout) const
{
    return layout.advance * sizeRatio;
}

LineLayout* VirtualFont::createLineLayout(const string &text, const string &langHint, hb_direction_t overallDirection)
{
    return createLineLayout(itemizer.processLine(text, langHint, overallDirection));
}

LineLayout* VirtualFont::createLineLayout(const TextLine &line)
{
    auto layout = new LineLayout(this, line.langHint, line.overallDirection);
    
    map<uint32_t, Cluster> clusterMap;
    auto buffer = hb_buffer_create();
    
    for (auto &run : line.runs)
    {
        clusterMap.clear();
        
        for (auto font : getFontSet(run.language))
        {
            if (font->hbFont)
            {
                layout->maxHeight = std::max(layout->maxHeight, font->metrics.height);
                layout->maxAscent = std::max(layout->maxAscent, font->metrics.ascent);
                layout->maxDescent = std::max(layout->maxDescent, font->metrics.descent);
                
                run.apply(line.text, buffer);
                hb_shape(font->hbFont, buffer, NULL, 0);
                
                auto glyphCount = hb_buffer_get_length(buffer);
                auto glyphInfos = hb_buffer_get_glyph_infos(buffer, NULL);
                auto glyphPositions = hb_buffer_get_glyph_positions(buffer, NULL);

                bool hasMissingGlyphs = false;
                
                for (int i = 0; i < glyphCount; i++)
                {
                    auto codepoint = glyphInfos[i].codepoint;
                    auto cluster = glyphInfos[i].cluster;
                    
                    auto it = clusterMap.find(cluster);
                    bool clusterFound = (it != clusterMap.end());
                    
                    if (codepoint)
                    {
                        if (clusterFound && (it->second.font != font))
                        {
                            continue; // CLUSTER FOUND, WITH ANOTHER FONT (E.G. SPACE)
                        }
                        else
                        {
                            auto offset = Vec2f(glyphPositions[i].x_offset, -glyphPositions[i].y_offset) * font->scale;
                            float advance = glyphPositions[i].x_advance * font->scale.x;
                            
                            if (clusterFound)
                            {
                                it->second.addShape(codepoint, offset, advance);
                            }
                            else
                            {
                                clusterMap.insert(make_pair(cluster, Cluster(font, codepoint, offset, advance)));
                            }
                        }
                    }
                    else if (!clusterFound)
                    {
                        hasMissingGlyphs = true;
                    }
                }
                
                if (!hasMissingGlyphs)
                {
                    break; // NO NEED TO PROCEED TO THE NEXT FONT IN THE LIST
                }
            }
        }
        
        if (run.direction == HB_DIRECTION_RTL)
        {
            for (auto it = clusterMap.rbegin(); it != clusterMap.rend(); ++it)
            {
                layout->addCluster(it->second);
            }
        }
        else
        {
            for (auto it = clusterMap.begin(); it != clusterMap.end(); ++it)
            {
                layout->addCluster(it->second);
            }
        }
    }
    
    hb_buffer_destroy(buffer);
    return layout;
}

void VirtualFont::setSize(float newSize)
{
    size = newSize;
    sizeRatio = newSize / baseSize;
}

void VirtualFont::setColor(const ColorA &color)
{
    colors.clear();
    colors.emplace_back(color);
    colors.emplace_back(color);
    colors.emplace_back(color);
    colors.emplace_back(color);
}

void VirtualFont::begin()
{
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(2, GL_FLOAT, 0, positions.data());
    glTexCoordPointer(2, GL_FLOAT, 0, coords.data());
    glColorPointer(4, GL_FLOAT, 0, colors.data());
}

void VirtualFont::end()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void VirtualFont::drawCluster(const Cluster &cluster, const Vec2f &position)
{
    for (auto shape : cluster.shapes)
    {
        auto glyph = cluster.font->getGlyph(shape.codepoint);
        
        if (glyph && glyph->texture)
        {
            auto ul = position + (shape.position + glyph->offset) * sizeRatio;
            auto lr = ul + glyph->size * sizeRatio;
            
            positions.clear();
            positions.emplace_back(ul);
            positions.emplace_back(lr.x, ul.y);
            positions.emplace_back(lr);
            positions.emplace_back(ul.x, lr.y);
            
            coords.clear();
            coords.emplace_back(glyph->tx1, glyph->ty1);
            coords.emplace_back(glyph->tx2, glyph->ty1);
            coords.emplace_back(glyph->tx2, glyph->ty2);
            coords.emplace_back(glyph->tx1, glyph->ty2);
            
            glyph->texture->bind();
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
    }
}
