/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "LayoutCache.h"

using namespace std;

LayoutCache::LayoutCache(size_t capacity)
:
capacity(capacity),
size(0)
{}

/*
 * TODO:
 * CURRENTLY, text WILL BE COPIED 2 OR 3 TIMES
 * CAN WE MAKE IT LESS?
 */
LineLayout* LayoutCache::getLineLayout(VirtualFont *virtualFont, const string &text, const string &langHint, hb_direction_t overallDirection)
{
    const LineLayoutKey key(virtualFont, text, langHint, overallDirection); // COPY #1 OF text
    auto it = cache.left.find(key); // COPY #2 OF text (A DRAWBACK OF Boost.bimap: key IS COPIED)
    
    if (it != cache.left.end())
    {
        /*
         * MOVING USED-ENTRY TO THE TAIL OF THE bimaps::list_of
         */
        cache.right.relocate(cache.right.end(), cache.project_right(it));
        return it->second.get();
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
                /*
                 * LEAST-RECENTLY-USED ENTRIES ARE AT THE HEAD OF THE bimaps::list_of
                 */
                size -= cache.right.begin()->second.text.size();
                cache.right.erase(cache.right.begin());
            }
        }

        size += newSize;

        /*
         * NEW ENTRIES ARE INSERTED AT THE TAIL OF THE bimaps::list_of
         */
        auto value = virtualFont->createLineLayout(text, langHint, overallDirection);
        cache.insert(typename container_type::value_type(key, shared_ptr<LineLayout>(value))); // COPY #3 OF text (ONLY IF key WAS NOT FOUND)
        
        return value;
    }
}

void LayoutCache::clear()
{
    cache.clear();
    size = 0;
}
