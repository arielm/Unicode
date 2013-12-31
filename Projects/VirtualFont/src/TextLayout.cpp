/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "TextLayout.h"

using namespace std;
using namespace ci;

Cluster::Cluster(ActualFont *font, hb_codepoint_t codepoint, const Vec2f &offset, float advance)
:
font(font),
combinedAdvance(advance)
{
    shapes.emplace_back(codepoint, offset);
}

void Cluster::addShape(hb_codepoint_t codepoint, const Vec2f &offset, float advance)
{
    shapes.emplace_back(codepoint, Vec2f(combinedAdvance, 0) + offset);
    combinedAdvance += advance;
}

TextLayout::TextLayout(const VirtualFont &virtualFont, const TextSpan &run)
:
direction(run.direction),
advance(0)
{
    process(virtualFont.getFontSet(run.lang), run);
}

void TextLayout::draw(const Vec2f &position)
{
    auto clusterPosition = position;

    for (auto cluster : clusters)
    {
        for (auto shape : cluster.shapes)
        {
            auto glyph = cluster.font->getGlyph(shape.codepoint);
            
            if (glyph && glyph->texture)
            {
                gl::draw(*glyph->texture, clusterPosition + shape.position + glyph->offset);
            }
        }
        
        clusterPosition.x += cluster.combinedAdvance;
    }
}

void TextLayout::addCluster(const Cluster &cluster)
{
    clusters.emplace_back(cluster);
    advance += cluster.combinedAdvance;
}

void TextLayout::process(const FontSet &fontSet, const TextSpan &run)
{
    auto buffer = hb_buffer_create();
    map<uint32_t, Cluster> clusterMap;
    
    for (auto font : fontSet)
    {
        hb_buffer_clear_contents(buffer);
        
        run.apply(buffer);
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
    
    if (run.direction == HB_DIRECTION_RTL)
    {
        for (auto it = clusterMap.rbegin(); it != clusterMap.rend(); ++it)
        {
            addCluster(it->second);
        }
    }
    else
    {
        for (auto it = clusterMap.begin(); it != clusterMap.end(); ++it)
        {
            addCluster(it->second);
        }
    }
    
    hb_buffer_destroy(buffer);

}