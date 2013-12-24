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
    std::shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES

    FontManager();
    
    YFont* getCachedFont(const std::string &ref, float fontSize);
    VirtualFont loadVirtualFont(ci::DataSourceRef source, float fontSize);
    
protected:
    std::map<FontKey, std::shared_ptr<YFont>> fontMap;
    
    static ci::fs::path getFilePath(const std::string &ref);
    static std::vector<std::string> getLanguageList(const std::string &languages);
};
