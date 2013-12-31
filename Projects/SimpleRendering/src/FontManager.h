/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "VirtualFont.h"

struct FontKey
{
    std::string ref;
    float baseSize;
    bool useMipmap;
    
    FontKey(const std::string &ref, float baseSize, bool useMipmap)
    :
    ref(ref),
    baseSize(baseSize),
    useMipmap(useMipmap)
    {}
    
    bool operator<(const FontKey &rhs) const
    {
        return tie(useMipmap, baseSize, ref) < tie(rhs.useMipmap, rhs.baseSize, rhs.ref);
    }
};

class FontManager
{
public:
    std::shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE ActualFont INSTANCES
    
    FontManager();
    
    ActualFont* getActualFont(const std::string &ref, float baseSize, bool useMipmap = true, int padding = 2);
    VirtualFont* getVirtualFont(const std::string &ref, float baseSize, bool useMipmap = true, int padding = 2);
    
protected:
    std::map<FontKey, std::unique_ptr<ActualFont>> actualFonts;
    std::map<FontKey, std::unique_ptr<VirtualFont>> virtualFonts;

    static std::vector<std::string> getLanguageList(const std::string &languages);
};
