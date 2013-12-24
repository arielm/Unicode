#pragma once

#include "YFont.h"

#include <set>
#include <map>
#include <string>

typedef std::set<YFont*> FontSet;

class VirtualFont
{
public:
    bool add(const std::string &lang, YFont *font);
    FontSet getFontSet(const std::string &lang);
    
protected:
    std::map<std::string, FontSet> fontSetMap;
};
