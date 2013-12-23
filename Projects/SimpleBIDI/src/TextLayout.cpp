#include "TextLayout.h"

using namespace std;
using namespace ci;

Cluster::Cluster(YFont *font, const ColorA &color, hb_codepoint_t codepoint, const Vec2f &offset, float advance)
:
font(font),
color(color),
combinedAdvance(advance)
{
    shapes.emplace_back(codepoint, offset);
}

void Cluster::addShape(hb_codepoint_t codepoint, const Vec2f &offset, float advance)
{
    shapes.emplace_back(codepoint, Vec2f(combinedAdvance, 0) + offset);
    combinedAdvance += advance;
}

TextLayout::TextLayout(map<hb_script_t, FontList> &fontMap, const TextSpan &span)
:
overallDirection(span.direction),
advance(0)
{
    process(fontMap, {span}); // TODO: CHECK IF IT WORKS IN VISUAL STUDIO 2012
}

TextLayout::TextLayout(map<hb_script_t, FontList> &fontMap, const TextGroup &group)
:
overallDirection(group.overallDirection),
advance(0)
{
    process(fontMap, group.runs);
}

void TextLayout::draw(const Vec2f &position)
{
    for (auto entry : clusters)
    {
        auto cluster = entry.first;
        auto clusterPosition = Vec2f(entry.second, 0) + position;
        
        for (auto shape : entry.first.shapes)
        {
            auto glyph = cluster.font->getGlyph(shape.codepoint);
            
            if (glyph && glyph->texture)
            {
                gl::color(cluster.color);
                gl::draw(glyph->texture, clusterPosition + shape.position + glyph->offset);
            }
        }
    }
}

void TextLayout::addCluster(const Cluster &cluster)
{
    clusters.emplace_back(cluster, advance);
    advance += cluster.combinedAdvance;
}

void TextLayout::process(map<hb_script_t, FontList> &fontMap, const vector<TextSpan> &runs)
{
    auto buffer = hb_buffer_create();
    map<uint32_t, Cluster> clusterMap;
    
    for (auto run : runs)
    {
        clusterMap.clear();
        
        for (auto font : fontMap[run.script])
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
                            const ColorA color = (run.direction == HB_DIRECTION_LTR) ? colorLTR : colorRTL;
                            clusterMap.insert(make_pair(cluster, Cluster(font.get(), color, codepoint, offset, advance)));
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
    }
    
    hb_buffer_destroy(buffer);
}
