#pragma once

#include "TextGroup.h"
#include "YFont.h"

const ci::ColorA colorLTR = ci::ColorA(1, 1, 1, 1);
const ci::ColorA colorRTL = ci::ColorA(1, 1, 0.5f, 1);

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

struct Cluster
{
    YFont *font;
    ci::ColorA color;
    
    float combinedAdvance;
    std::vector<Shape> shapes;
    
    Cluster(YFont *font, const ci::ColorA &color, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
    void addShape(hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
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
    
    void draw(const ci::Vec2f &position);
    
protected:
    void addCluster(const Cluster &cluster);
    void process(std::map<hb_script_t, FontList> &fontMap, const std::vector<TextSpan> &runs);
};
