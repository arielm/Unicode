/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "FontManager.h"

#include "chronotext/utils/Utils.h"

#include "cinder/Xml.h"

using namespace std;
using namespace ci;
using namespace chr;

enum
{
    PLATFORM_OSX,
    PLATFORM_WINDOW,
    PLATFORM_IOS,
    PLATFORM_ANDROID
};

const string PLATFORM_NAMES[4] = {"osx", "windows", "ios", "android"};

FontManager::FontManager()
{
    ftHelper = make_shared<FreetypeHelper>();

#if defined(CINDER_MAC)
    platform = PLATFORM_OSX;
#elif defined(CINDER_MSW)
    platform = PLATFORM_WINDOW;
#elif defined(CINDER_COCOA_TOUCH)
    platform = PLATFORM_IOS;
#elif defined(CINDER_ANDROID)
    platform = PLATFORM_ANDROID;
#else
    throw;
#endif
}

void FontManager::loadDefinitions(InputSourceRef source)
{
    XmlTree doc(source->loadDataSource()); // EARLY-THROW IF THE DOCUMENT IS MALFORMED
    
    for (auto fontElement : doc.getChild("Fonts"))
    {
        auto name = fontElement.getAttributeValue<string>("name");
        int style = parseStyle(fontElement.getAttributeValue<string>("style", "plain"));
        float baseSize = fontElement.getAttributeValue<float>("base-size", 0);
        
        for (auto &fileElement : fontElement.getChildren())
        {
            auto os = fileElement->getAttributeValue<string>("os");
            
            if (os == PLATFORM_NAMES[platform])
            {
                auto uri = fileElement->getAttributeValue<string>("uri");
                definitions[make_pair(name, style)] = make_pair(uri, baseSize);
                break;
            }
        }
    }
}

VirtualFont* FontManager::getFont(const std::string &name, int style, float baseSize)
{
    auto it = definitions.find(make_pair(name, style));
    
    if (it != definitions.end())
    {
        bool useMipmap = false;
        
        if (baseSize == 0)
        {
            baseSize = it->second.second;
            useMipmap = true;
        }
        
        if (baseSize == 0)
        {
            throw invalid_argument("INVALID FONT-SIZE");
        }
        
        auto uri = it->second.first;
        return getFont(InputSource::get(uri), baseSize, useMipmap);
    }
    
    throw invalid_argument("UNDEFINED FONT");;
}

VirtualFont* FontManager::getFont(InputSourceRef source, float baseSize, bool useMipmap)
{
    FontKey key(source->getURI(), baseSize, useMipmap);
    auto it = virtualFonts.find(key);
    
    if (it != virtualFonts.end())
    {
        return it->second.get();
    }
    else
    {
        XmlTree doc(source->loadDataSource()); // EARLY-THROW IF THE DOCUMENT IS MALFORMED
        
        auto font = new VirtualFont(itemizer, baseSize);
        virtualFonts[key] = unique_ptr<VirtualFont>(font);
        
        for (auto fontElement : doc.getChild("VirtualFont"))
        {
            auto langList = splitLanguages(fontElement.getAttributeValue<string>("lang", ""));
            
            for (auto lang : langList)
            {
                for (auto &variantElement : fontElement.getChildren())
                {
                    if (variantElement->getTag() == "Group")
                    {
                        for (auto &refElement : variantElement->getChildren())
                        {
                            string uri = refElement->getValue();
                            
                            if (font->add(lang, getActualFont(uri, baseSize, useMipmap)))
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        string uri = variantElement->getValue();
                        font->add(lang, getActualFont(uri, baseSize, useMipmap));
                    }
                }
            }
        }
        
        return font;
    }
}

void FontManager::reload()
{
    for (auto &it : actualFonts)
    {
        it.second->reload();
    }
}

void FontManager::unload()
{
    for (auto &it : actualFonts)
    {
        it.second->unload();
    }
}

void FontManager::discardTextures()
{
    for (auto &it : actualFonts)
    {
        it.second->discardTextures();
    }
}

ActualFont* FontManager::getActualFont(const string &uri, float baseSize, bool useMipmap)
{
    FontKey key(uri, baseSize, useMipmap);
    auto it = actualFonts.find(key);
    
    if (it != actualFonts.end())
    {
        return it->second.get();
    }
    else
    {
        try
        {
            auto font = new ActualFont(ftHelper, InputSource::get(uri), baseSize, useMipmap);
            actualFonts[key] = unique_ptr<ActualFont>(font);
            
            return font;
        }
        catch (exception &e)
        {
            LOGD << e.what() << " - " << uri << endl;
        }
        
        return NULL;
    }
}

vector<string> FontManager::splitLanguages(const string &languages)
{
	return split(languages, ":");
}

int FontManager::parseStyle(const string &style)
{
    if (style == "bold") return VirtualFont::STYLE_BOLD;
    if (style == "italic") return VirtualFont::STYLE_ITALIC;
    if (style == "bold-italic") return VirtualFont::STYLE_BOLD_ITALIC;
    
    return VirtualFont::STYLE_PLAIN;
}
