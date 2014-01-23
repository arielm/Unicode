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
    typedef enum
    {
        STYLE_REGULAR,
        STYLE_BOLD,
        STYLE_ITALIC,
        STYLE_BOLD_ITALIC
    }
    Style;
    
    typedef enum
    {
        ALIGN_MIDDLE,
        ALIGN_LEFT,
        ALIGN_RIGHT,
        ALIGN_TOP,
        ALIGN_BASELINE,
        ALIGN_BOTTOM
    }
    Alignment;
    
    struct Key
    {
        std::string uri;
        float baseSize;
        bool useMipmap;
        
        Key(const std::string &uri, float baseSize, bool useMipmap)
        :
        uri(uri),
        baseSize(baseSize),
        useMipmap(useMipmap)
        {}
        
        bool operator<(const Key &rhs) const
        {
            return tie(uri, baseSize, useMipmap) < tie(rhs.uri, rhs.baseSize, rhs.useMipmap);
        }
    };
    
    TextItemizer &itemizer;
    float baseSize;
    
    VirtualFont(TextItemizer &itemizer, float baseSize);
    
    bool add(const std::string &lang, ActualFont *font);
    const FontSet& getFontSet(const std::string &lang) const;
    
    float getAscent(const LineLayout &layout) const;
    float getDescent(const LineLayout &layout) const;
    float getMiddleLine(const LineLayout &layout) const;
    
    float getOffsetX(const LineLayout &layout, Alignment align) const;
    float getOffsetY(const LineLayout &layout, Alignment align) const;

    float getAdvance(const LineLayout &layout) const;
    float getAdvance(const Cluster &cluster) const;

    ActualFont::Metrics getMetrics(const Cluster &cluster) const;
    ActualFont::Metrics getMetrics(const std::string &lang) const;

    /*
     * THE RETURNED LineLayout INSTANCES ARE NOT MANAGED AND SHOULD BE DELETED BY THE CALLER
     */
    LineLayout* createLineLayout(const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_INVALID);
    LineLayout* createLineLayout(const TextLine &line);
    
    void setSize(float size);
    void setColor(const ci::ColorA &color);
    
    void begin();
    void end();
    void drawCluster(const Cluster &cluster, const ci::Vec2f &position);

protected:
    float size;
    float sizeRatio;

    std::vector<ci::Vec2f> positions;
    std::vector<ci::Vec2f> coords;
    std::vector<ci::ColorA> colors;
    
    FontSet defaultFontSet; // ALLOWING getFontSet() TO RETURN CONST VALUES
    std::map<std::string, FontSet> fontSetMap;
};
