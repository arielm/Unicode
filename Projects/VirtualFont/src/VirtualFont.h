/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

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
