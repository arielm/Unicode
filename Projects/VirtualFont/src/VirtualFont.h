/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "ActualFont.h"
#include "LineLayout.h"
#include "TextItemizer.h"

#include <set>
#include <map>

typedef std::vector<ActualFont*> FontSet;

class VirtualFont
{
public:
    enum
    {
        STYLE_REGULAR,
        STYLE_BOLD,
        STYLE_ITALIC,
        STYLE_BOLD_ITALIC
    };
    
    TextItemizer &itemizer;
    float baseSize;
    
    VirtualFont(TextItemizer &itemizer, float baseSize);
    
    bool add(const std::string &lang, ActualFont *font);
    const FontSet& getFontSet(const std::string &lang) const;
    const ActualFont::Metrics& getMetrics(const std::string &lang) const;
    
    LineLayout* createLineLayout(const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_LTR);
    LineLayout* createLineLayout(const TextLine &line);
    
    void setSize(float size);
    void setColor(const ci::ColorA &color);
    
    float getAdvance(const Cluster &cluster) const;
    float getAdvance(const LineLayout &layout) const;
    
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
