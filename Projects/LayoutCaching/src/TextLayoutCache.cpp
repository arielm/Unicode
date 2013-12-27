#include "TextLayoutCache.h"

using namespace std;

TextLayout* TextLayoutCache::get(VirtualFont *virtualFont, const TextSpan &run)
{
    const TextLayoutKey key(virtualFont, run);
    auto it = cache.find(key);
    
    if (it != cache.end())
    {
        return it->second.get();
    }
    else
    {
        auto value = new TextLayout(virtualFont, run);
        cache[key] = shared_ptr<TextLayout>(value);
        return value;
    }
}
