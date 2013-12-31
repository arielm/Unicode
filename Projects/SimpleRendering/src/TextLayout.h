/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "hb.h"

#include "cinder/Vector.h"

#include <vector>

class ActualFont;
class VirtualFont;

struct Shape
{
    hb_codepoint_t codepoint;
    ci::Vec2f position;
    
    Shape(hb_codepoint_t codepoint, const ci::Vec2f &position)
    :
    codepoint(codepoint),
    position(position)
    {}
};

struct Cluster
{
    ActualFont *font;
    
    float combinedAdvance;
    std::vector<Shape> shapes;
    
    Cluster(ActualFont *font, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance)
    :
    font(font),
    combinedAdvance(advance)
    {
        shapes.emplace_back(codepoint, offset);
    }
    
    void addShape(hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance)
    {
        shapes.emplace_back(codepoint, ci::Vec2f(combinedAdvance, 0) + offset);
        combinedAdvance += advance;
    }
};

struct TextLayout
{
    VirtualFont *font;
    std::string lang;
    hb_direction_t direction;
    
    float advance;
    std::vector<Cluster> clusters;
    
    TextLayout(VirtualFont *font, const std::string lang, hb_direction_t direction)
    :
    font(font),
    lang(lang),
    direction(direction),
    advance(0)
    {}
    
    void addCluster(const Cluster &cluster)
    {
        clusters.emplace_back(cluster);
        advance += cluster.combinedAdvance;
    }
};
