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

const int stride = sizeof(Vec2f) * 2;

VirtualFont::VirtualFont(LayoutCache &layoutCache, TextItemizer &itemizer, float baseSize)
:
layoutCache(layoutCache),
itemizer(itemizer),
baseSize(baseSize)
{
    vertices.reserve(4 * 2);
    colors.reserve(4);
    
    setSize(baseSize);
    setColor(ColorA(0, 0, 0, 1));
}

bool VirtualFont::addActualFont(const string &lang, shared_ptr<ActualFont> font)
{
    if (font)
    {
        /*
         * WHY USING std::vector INSTEAD OF std::set?
         * BECAUSE ORDER OF INSERTION MATTERS
         */
        
        for (auto &tmp : fontSetMap[lang])
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

ActualFont::Metrics VirtualFont::getMetrics(const Cluster &cluster) const
{
    return cluster.font->metrics * sizeRatio;
}

ActualFont::Metrics VirtualFont::getMetrics(const string &lang) const
{
    auto &fontSet = getFontSet(lang);
    
    if (fontSet.empty())
    {
        return ActualFont::Metrics();
    }
    else
    {
        return fontSet.front()->metrics * sizeRatio;
    }
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

Vec2f VirtualFont::getOffset(const LineLayout &layout, Alignment alignX, Alignment alignY) const
{
    return Vec2f(getOffsetX(layout, alignX), getOffsetY(layout, alignY));
}

float VirtualFont::getAdvance(const LineLayout &layout) const
{
    return layout.advance * sizeRatio;
}

float VirtualFont::getAdvance(const Cluster &cluster) const
{
    return cluster.combinedAdvance * sizeRatio;
}

float VirtualFont::getHeight(const LineLayout &layout) const
{
    return layout.maxHeight * sizeRatio;
}

float VirtualFont::getAscent(const LineLayout &layout) const
{
    return layout.maxAscent * sizeRatio;
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
        
        for (auto &font : getFontSet(run.language))
        {
            font->reload();
            
            if (font->loaded)
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
                        if (clusterFound && (it->second.font != font.get()))
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
                                clusterMap.insert(make_pair(cluster, Cluster(font.get(), codepoint, offset, advance)));
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

shared_ptr<LineLayout> VirtualFont::getCachedLineLayout(const string &text, const string &langHint, hb_direction_t overallDirection)
{
    return layoutCache.getLineLayout(this, text, langHint, overallDirection);
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

    glVertexPointer(2, GL_FLOAT, stride, vertices.data());
    glTexCoordPointer(2, GL_FLOAT, stride, vertices.data() + 1);
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
    for (auto &shape : cluster.shapes)
    {
        auto glyph = cluster.font->getGlyph(shape.codepoint);
        
        if (glyph && glyph->texture)
        {
            auto ul = position + (shape.position + glyph->offset) * sizeRatio;
            auto lr = ul + glyph->size * sizeRatio;
            
            vertices.clear();
            
            vertices.emplace_back(ul);
            vertices.emplace_back(glyph->tx1, glyph->ty1);

            vertices.emplace_back(lr.x, ul.y);
            vertices.emplace_back(glyph->tx2, glyph->ty1);

            vertices.emplace_back(lr);
            vertices.emplace_back(glyph->tx2, glyph->ty2);

            vertices.emplace_back(ul.x, lr.y);
            vertices.emplace_back(glyph->tx1, glyph->ty2);
            
            glyph->texture->bind();
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
    }
}

VirtualFont::Style VirtualFont::styleStringToEnum(const string &style)
{
    if (style == "bold") return VirtualFont::STYLE_BOLD;
    if (style == "italic") return VirtualFont::STYLE_ITALIC;
    if (style == "bold-italic") return VirtualFont::STYLE_BOLD_ITALIC;
    
    return VirtualFont::STYLE_REGULAR; // DEFAULT
}

string VirtualFont::styleEnumToString(VirtualFont::Style style)
{
    switch (style)
    {
        case VirtualFont::STYLE_BOLD:
            return "bold";
            
        case VirtualFont::STYLE_ITALIC:
            return "italic";
            
        case VirtualFont::STYLE_BOLD_ITALIC:
            return "bold-italic";
            
        default:
        case VirtualFont::STYLE_REGULAR:
            return "regular";
    }
}
