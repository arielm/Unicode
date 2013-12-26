/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "VirtualFont.h"

#include "cinder/DataSource.h"

struct FontKey
{
    std::string ref;
    float fontSize;
    
    FontKey(const std::string &ref, float fontSize)
    :
    ref(ref),
    fontSize(fontSize)
    {}
    
    bool operator<(const FontKey &rhs) const
    {
        if (fontSize == rhs.fontSize)
        {
            return (ref < rhs.ref);
        }
        else
        {
            return (fontSize < rhs.fontSize);
        }
    }
};

class FontManager
{
public:
    std::shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE ActualFont INSTANCES

    FontManager();
    
    ActualFont* getCachedFont(const std::string &ref, float fontSize);
    VirtualFont loadVirtualFont(ci::DataSourceRef source, float fontSize);
    
protected:
    std::map<FontKey, std::shared_ptr<ActualFont>> fontMap;
    
    static ci::fs::path getFilePath(const std::string &ref);
    static std::vector<std::string> getLanguageList(const std::string &languages);
};
