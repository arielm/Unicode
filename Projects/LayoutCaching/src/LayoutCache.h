/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "LineLayout.h"
#include "VirtualFont.h"

#include <map>
#include <memory>

class LayoutCache
{
public:
    struct LineLayoutKey
    {
        VirtualFont *virtualFont;
        std::string text;
        std::string langHint;
        hb_direction_t overallDirection;
        
        LineLayoutKey(VirtualFont *virtualFont, const std::string &text, const std::string &langHint, hb_direction_t overallDirection)
        :
        virtualFont(virtualFont),
        text(text),
        langHint(langHint),
        overallDirection(overallDirection)
        {}
        
        bool operator<(const LineLayoutKey &rhs) const
        {
            return tie(virtualFont, overallDirection, langHint, text) < tie(rhs.virtualFont, rhs.overallDirection, rhs.langHint, rhs.text);
        }
    };
    
    LayoutCache(size_t capacity = 32 * 1024);
    
    LineLayout* getLineLayout(VirtualFont *virtualFont, const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_LTR);
    void clear();

protected:
    size_t capacity;
    size_t size;
    size_t time;
    
    std::map<LineLayoutKey, std::unique_ptr<LineLayout>> cache;
    std::map<LineLayout*, size_t> usageMap;
    
    size_t removeOldest();
};
