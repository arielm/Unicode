/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "TextLayout.h"
#include "VirtualFont.h"

class TextLayoutKey
{
public:
    VirtualFont *virtualFont;
    TextRun run;
    
    TextLayoutKey(VirtualFont *virtualFont, const TextRun &run)
    :
    virtualFont(virtualFont),
    run(run)
    {}
    
    bool operator<(const TextLayoutKey &rhs) const
    {
        if (virtualFont == rhs.virtualFont)
        {
            return (run < rhs.run);
        }
        else
        {
            return (virtualFont < rhs.virtualFont);
        }
    }
};

class TextLayoutCache
{
public:
    TextLayout* get(VirtualFont *virtualFont, const TextRun &run);
    void purge();
    
protected:
    std::map<TextLayoutKey, std::unique_ptr<TextLayout>> cache;
};
