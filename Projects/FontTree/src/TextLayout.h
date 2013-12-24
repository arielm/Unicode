#pragma once

#include "TextSpan.h"
#include "YFont.h"

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
    YFont *font;
    float combinedAdvance;
    std::vector<Shape> shapes;
    
    Cluster(YFont *font, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
    void addShape(hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
};

class TextLayout
{
public:
    hb_direction_t direction;
    float advance;
    std::vector<std::pair<Cluster, float>> clusters;
    
    TextLayout(const FontSet &fontSet, const TextSpan &span);
    void draw(const ci::Vec2f &position);
    
protected:
    void addCluster(const Cluster &cluster);
    void process(const FontSet &fontSet, const TextSpan &span);
};
