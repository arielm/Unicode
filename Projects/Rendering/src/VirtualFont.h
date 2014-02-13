/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "ActualFont.h"
#include "LayoutCache.h"
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
    
    LayoutCache &layoutCache;
    TextItemizer &itemizer;
    float baseSize;

    ActualFont::Metrics getMetrics(const Cluster &cluster) const; // RETURNS THE SIZED METRICS OF THE ActualFont USED BY cluster
    ActualFont::Metrics getMetrics(const std::string &lang = "") const; // RETURNS THE SIZED METRICS OF THE FIRST ActualFont IN THE SET USED FOR lang
    
    float getHeight(const LineLayout &layout) const;
    float getAscent(const LineLayout &layout) const;
    float getDescent(const LineLayout &layout) const;
    float getMiddleLine(const LineLayout &layout) const;
    
    float getOffsetX(const LineLayout &layout, Alignment align) const;
    float getOffsetY(const LineLayout &layout, Alignment align) const;
    inline ci::Vec2f getOffset(const LineLayout &layout, Alignment alignX, Alignment alignY) const { return ci::Vec2f(getOffsetX(layout, alignX), getOffsetY(layout, alignY)); }

    float getAdvance(const LineLayout &layout) const;
    float getAdvance(const Cluster &cluster) const;

    /*
     * THE RETURNED INSTANCES ARE NOT MANAGED AND SHOULD BE DELETED BY THE CALLER
     */
    LineLayout* createLineLayout(const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_INVALID);
    LineLayout* createLineLayout(const TextLine &line);
    
    std::shared_ptr<LineLayout> getCachedLineLayout(const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_INVALID);
    
    void setSize(float size);
    void setColor(const ci::ColorA &color);
    
    void begin();
    void end();
    void drawCluster(const Cluster &cluster, const ci::Vec2f &position);
    
    static Style styleStringToEnum(const std::string &style);
    static std::string styleEnumToString(Style style);

protected:
    float size;
    float sizeRatio;

    std::vector<ci::Vec2f> vertices;
    std::vector<ci::ColorA> colors;
    
    FontSet defaultFontSet; // ALLOWING getFontSet() TO RETURN CONST VALUES
    std::map<std::string, FontSet> fontSetMap;
    
    VirtualFont(LayoutCache &layoutCache, TextItemizer &itemizer, float baseSize);
    
    bool addActualFont(const std::string &lang, ActualFont *font);
    const FontSet& getFontSet(const std::string &lang) const;
    
    friend class FontManager;
};
