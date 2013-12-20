#pragma once

#include "TextGroup.h"
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
    hb_direction_t overallDirection;
    float advance;
    std::vector<std::pair<Cluster, float>> clusters;
    
    TextLayout() {}
    TextLayout(std::map<hb_script_t, FontList> &fontMap, const TextSpan &span);
    TextLayout(std::map<hb_script_t, FontList> &fontMap, const TextGroup &group);

    void addCluster(const Cluster &cluster);
    void draw(const ci::Vec2f &position);
    
protected:
    void process(std::map<hb_script_t, FontList> &fontMap, const std::vector<TextSpan> &runs);
};
