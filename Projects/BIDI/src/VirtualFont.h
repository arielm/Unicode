/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "ActualFont.h"
#include "TextLayout.h"
#include "TextGroup.h"

#include <set>
#include <map>

typedef std::vector<ActualFont*> FontSet;

class VirtualFont
{
public:
    float baseSize;
    
    VirtualFont(float baseSize);
    
    bool add(const std::string &lang, ActualFont *font);
    const FontSet& getFontSet(const std::string &lang) const;
    const ActualFont::Metrics& getMetrics(const std::string &lang) const;
    
    TextLayout* createTextLayout(const TextGroup &group);
    
    void setSize(float size);
    
    float getAdvance(const Cluster &cluster) const;
    float getAdvance(const TextLayout &layout) const;
    
    void begin();
    void end();
    void drawCluster(const Cluster &cluster, const ci::Vec2f &position);

protected:
    float size;
    float sizeRatio;

    std::vector<ci::Vec2f> positions;
    std::vector<ci::Vec2f> coords;
    std::vector<ci::ColorA> colors;
    
    FontSet defaultFontSet;
    ActualFont::Metrics defaultMetrics;
    
    std::map<std::string, FontSet> fontSetMap;
};
