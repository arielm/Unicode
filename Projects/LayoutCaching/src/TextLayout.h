/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "TextRun.h"
#include "VirtualFont.h"

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
    
    Cluster(ActualFont *font, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
    void addShape(hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
};

class TextLayout
{
public:
    hb_direction_t direction;
    float advance;
    std::vector<std::pair<Cluster, float>> clusters;
    
    TextLayout(VirtualFont *virtualFont, const TextRun &run);
    void draw(const ci::Vec2f &position);
    
protected:
    void addCluster(const Cluster &cluster);
};
