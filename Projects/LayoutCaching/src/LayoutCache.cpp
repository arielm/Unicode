/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "LayoutCache.h"

#include <limits> 

using namespace std;

LayoutCache::LayoutCache(size_t capacity)
:
capacity(capacity),
size(0),
time(0)
{}

LineLayout* LayoutCache::getLineLayout(VirtualFont *virtualFont, const string &text, const string &langHint, hb_direction_t overallDirection)
{
    LineLayout *value;

    const LineLayoutKey key(virtualFont, text, langHint, overallDirection);
    auto it = cache.find(key);
    
    if (it != cache.end())
    {
        value = it->second.get();
    }
    else
    {
        auto newSize = text.size();
        
        if (newSize >= capacity)
        {
            clear();
        }
        else
        {
            while (size + newSize > capacity)
            {
                size -= removeOldest();
            }
        }

        size += newSize;

        value = virtualFont->createLineLayout(text, langHint, overallDirection);
        cache[key] = unique_ptr<LineLayout>(value);
    }
    
    usageMap[value] = ++time;
    return value;
}

void LayoutCache::clear()
{
    cache.clear();
    usageMap.clear();
    
    size = time = 0;
}

/*
 * FIXME: NOT VERY EFFECTIVE
 */
size_t LayoutCache::removeOldest()
{
    LineLayout *oldest = NULL;
    size_t earliest = numeric_limits<size_t>::max();
    
    for (auto &it : usageMap)
    {
        if (it.second < earliest)
        {
            oldest = it.first;
            earliest = it.second;
        }
    }
    
    if (oldest)
    {
        usageMap.erase(oldest);
        
        for (auto it = cache.begin(); it != cache.end(); ++it)
        {
            if (it->second.get() == oldest)
            {
                size_t gain = it->first.text.size();
                cache.erase(it);
                return gain;
            }
        }
    }
    
    return 0;
}
