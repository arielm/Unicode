/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "FontManager.h"

#include "chronotext/InputSource.h"
#include "chronotext/utils/Utils.h"

#include "cinder/Xml.h"

using namespace std;
using namespace ci;
using namespace chr;

FontManager::FontManager()
{
    ftHelper = make_shared<FreetypeHelper>();
}

ActualFont* FontManager::getActualFont(const string &uri, float baseSize, bool useMipmap, int padding)
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
            auto font = new ActualFont(ftHelper, InputSource::get(uri), baseSize, useMipmap, padding);
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

VirtualFont* FontManager::getVirtualFont(const string &uri, float baseSize, bool useMipmap, int padding)
{
    FontKey key(uri, baseSize, useMipmap);
    auto it = virtualFonts.find(key);
    
    if (it != virtualFonts.end())
    {
        return it->second.get();
    }
    else
    {
        auto font = new VirtualFont(itemizer, baseSize);
        virtualFonts[key] = unique_ptr<VirtualFont>(font);
        
        XmlTree doc(InputSource::load(uri));
        auto rootElement = doc.getChild("VirtualFont");
        
        for (auto &fontElement : rootElement.getChildren())
        {
            auto langList = getLanguageList(fontElement->getAttributeValue<string>("lang", ""));
            
            for (auto &lang : langList)
            {
                for (auto &variantElement : fontElement->getChildren())
                {
                    if (variantElement->getTag() == "Group")
                    {
                        for (auto &refElement : variantElement->getChildren())
                        {
                            string uri = refElement->getValue();
                            
                            if (font->add(lang, getActualFont(uri, baseSize, useMipmap, padding)))
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        string uri = variantElement->getValue();
                        font->add(lang, getActualFont(uri, baseSize, useMipmap, padding));
                    }
                }
            }
        }
        
        return font;
    }
}

void FontManager::unloadTextures()
{
    for (auto &it : actualFonts)
    {
        it.second->unloadTextures();
    }
}

vector<string> FontManager::getLanguageList(const string &languages)
{
	return split(languages, ":");
}
