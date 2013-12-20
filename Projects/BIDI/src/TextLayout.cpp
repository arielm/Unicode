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
    gl::color(font->color);
    
    for (auto shape : shapes)
    {
        YGlyph *glyph = font->getGlyph(shape.codepoint);
        
        if (glyph && glyph->texture)
        {
            gl::draw(glyph->texture, position + shape.position + glyph->offset);
        }
    }
    
    return combinedAdvance;
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

void TextLayout::process(map<hb_script_t, FontList> &fontMap, const vector<TextSpan> &runs)
{
    hb_buffer_t *buffer = hb_buffer_create();
    
    float combinedAdvance = 0;
    map<uint32_t, Cluster> runClusters;
    
    for (auto run : runs)
    {
        runClusters.clear();
        
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
                
                auto it = runClusters.find(cluster);
                bool clusterFound = (it != runClusters.end());
                
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
                            runClusters.insert(make_pair(cluster, Cluster(font.get(), codepoint, offset, advance)));
                        }
                        
                        combinedAdvance += advance;
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
            for (auto it = runClusters.rbegin(); it != runClusters.rend(); ++it)
            {
                addCluster(it->second);
            }
        }
        else
        {
            for (auto it = runClusters.begin(); it != runClusters.end(); ++it)
            {
                addCluster(it->second);
            }
        }
    }
    
    hb_buffer_destroy(buffer);
}
