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
    bool useMipmap;
    
    FontKey(const std::string &ref, float fontSize, bool useMipmap)
    :
    ref(ref),
    fontSize(fontSize),
    useMipmap(useMipmap)
    {}
    
    bool operator<(const FontKey &rhs) const
    {
        return tie(useMipmap, fontSize, ref) < tie(rhs.useMipmap, rhs.fontSize, rhs.ref);
    }
};

class FontManager
{
public:
    std::shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE ActualFont INSTANCES
    
    FontManager();
    
    ActualFont* getActualFont(const std::string &ref, float fontSize, bool useMipmap = false);
    VirtualFont* getVirtualFont(const std::string &ref, float fontSize, bool useMipmap = false);
    
protected:
    std::map<FontKey, std::shared_ptr<ActualFont>> actualFonts;
    std::map<FontKey, std::shared_ptr<VirtualFont>> virtualFonts;

    static std::vector<std::string> getLanguageList(const std::string &languages);
    static ci::fs::path getFilePath(const std::string &ref);
    static ci::DataSourceRef getDataSource(const std::string &ref);
};
