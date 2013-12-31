/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "TextLayoutCache.h"

using namespace std;

TextLayout* TextLayoutCache::get(VirtualFont *virtualFont, const TextRun &run)
{
    const TextLayoutKey key(virtualFont, run);
    auto it = cache.find(key);
    
    if (it != cache.end())
    {
        return it->second.get();
    }
    else
    {
        auto value = virtualFont->createTextLayout(run);
        cache[key] = shared_ptr<TextLayout>(value);
        return value;
    }
}

void TextLayoutCache::purge()
{
    cache.clear();
}
