#pragma once

#include "YFont.h"

struct Shape
{
    hb_codepoint_t codepoint;
    ci::Vec2f position;
    float advance;
    
    Shape(uint32_t codepoint, const ci::Vec2f &position, float advance)
    :
    codepoint(codepoint),
    position(position),
    advance(advance)
    {}
};

class Cluster
{
public:
    YFont *font;
    float combinedAdvance;
    std::vector<Shape> shapes;
    
    Cluster(YFont *font, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
    
    void addShape(hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
    float draw(const ci::Vec2f &position);
};

class TextLayout
{
public:
    float advance;
    std::vector<std::pair<Cluster, float>> clusters;
    
    TextLayout();

    void addCluster(const Cluster &cluster);
    void draw(const ci::Vec2f &position);
};
