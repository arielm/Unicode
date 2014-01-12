/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "LayoutCache.h"

using namespace std;

LineLayout* LayoutCache::getLineLayout(VirtualFont *virtualFont, const string &text, const string &langHint, hb_direction_t overallDirection)
{
    const LineLayoutKey key(virtualFont, text, langHint, overallDirection);
    auto it = cache.find(key);
    
    if (it != cache.end())
    {
        return it->second.get();
    }
    else
    {
        auto value = virtualFont->createLineLayout(text, langHint, overallDirection);
        cache[key] = unique_ptr<LineLayout>(value);
        return value;
    }
}

void LayoutCache::clear()
{
    cache.clear();
}
