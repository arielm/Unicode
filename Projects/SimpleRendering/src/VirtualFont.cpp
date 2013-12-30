/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "VirtualFont.h"

using namespace std;

bool VirtualFont::add(const string &lang, ActualFont *font)
{
    if (font)
    {
        fontSetMap[lang].insert(font);
        return true;
    }
    
    return false;
}

FontSet VirtualFont::getFontSet(const string &lang) const
{
    auto it = fontSetMap.find(lang);
    
    if (it == fontSetMap.end())
    {
        it = fontSetMap.find("");
        
        if (it == fontSetMap.end())
        {
            return FontSet();
        }
    }
    
    return it->second;
}

ActualFont* VirtualFont::getDefaultFont(const string &lang) const
{
    auto fontSet = getFontSet(lang);
    
    if (fontSet.empty())
    {
        return NULL;
    }
    else
    {
        return *fontSet.begin();
    }
}