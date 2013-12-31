/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "ActualFont.h"

#include <set>
#include <map>
#include <string>

class Cluster;
class TextLayout;

typedef std::set<ActualFont*> FontSet;

class VirtualFont
{
public:
    float baseSize;
    
    VirtualFont(float baseSize);
    
    bool add(const std::string &lang, ActualFont *font);
    const FontSet& getFontSet(const std::string &lang) const;
    const ActualFont::Metrics& getMetrics(const std::string &lang) const;
    
    void setSize(float newSize);
    float getAdvance(const Cluster &cluster) const;
    float getAdvance(const TextLayout &layout) const;
    void drawCluster(const Cluster &cluster, const ci::Vec2f &position);

protected:
    float size;
    float sizeRatio;
    
    std::map<std::string, FontSet> fontSetMap;
};
