/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "ActualFont.h"

#include <set>
#include <map>
#include <string>

typedef std::set<ActualFont*> FontSet;

class VirtualFont
{
public:
    bool add(const std::string &lang, ActualFont *font);
    FontSet getFontSet(const std::string &lang) const;
    ActualFont* getDefaultFont(const std::string &lang) const;
    
protected:
    std::map<std::string, FontSet> fontSetMap;
};
