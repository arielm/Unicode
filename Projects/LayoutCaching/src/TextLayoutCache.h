#pragma once

#include "TextLayout.h"

class TextLayoutKey
{
public:
    VirtualFont *virtualFont;
    TextSpan run;
    
    TextLayoutKey(VirtualFont *virtualFont, const TextSpan &run)
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
    TextLayout* get(VirtualFont *virtualFont, const TextSpan &run);
    
protected:
    std::map<TextLayoutKey, std::shared_ptr<TextLayout>> cache;
};
