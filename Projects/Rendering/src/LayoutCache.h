/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * CACHE STRATEGY BASED ON "LRU cache implementation in C++" BY TIM DAY
 * http://timday.bitbucket.org/lru.html
 */

#pragma once

#include "LineLayout.h"

#include <boost/bimap.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/set_of.hpp>

#include <memory>

class VirtualFont;

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
    
    /*
     * THE CACHED INSTANCES ARE MANAGED BY LayoutCache AND WILL BE VALID AS LONG AS THE LATTER IS ALIVE
     */
    std::shared_ptr<LineLayout> getLineLayout(VirtualFont *virtualFont, const std::string &text, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_INVALID);

    void clear();
    void setCapacity(size_t newCapacity);
    size_t getMemoryUsage() const;

protected:
    typedef boost::bimaps::bimap<
    boost::bimaps::set_of<LineLayoutKey>,
    boost::bimaps::list_of<std::shared_ptr<LineLayout>>
    > container_type;

    size_t capacity;
    size_t size;
    container_type cache;
};
