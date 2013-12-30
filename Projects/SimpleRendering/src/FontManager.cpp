/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "FontManager.h"
#include "InputSource.h"

#include "cinder/Xml.h"
#include "cinder/Utilities.h"

using namespace std;
using namespace ci;

FontManager::FontManager()
{
    ftHelper = make_shared<FreetypeHelper>();
}

ActualFont* FontManager::getActualFont(const string &ref, float fontSize, bool useMipmap)
{
    FontKey key(ref, fontSize, useMipmap);
    auto it = actualFonts.find(key);
    
    if (it != actualFonts.end())
    {
        return it->second.get();
    }
    else
    {
        auto filePath = InputSource::getFilePath(ref);
        
        if (!filePath.empty())
        {
            auto font = new ActualFont(ftHelper, filePath, fontSize, useMipmap);
            actualFonts[key] = shared_ptr<ActualFont>(font);
            
            return font;
        }
        else
        {
            return NULL;
        }
    }
}

VirtualFont* FontManager::getVirtualFont(const string &ref, float fontSize, bool useMipmap)
{
    FontKey key(ref, fontSize, useMipmap);
    auto it = virtualFonts.find(key);
    
    if (it != virtualFonts.end())
    {
        return it->second.get();
    }
    else
    {
        auto font = new VirtualFont();
        virtualFonts[key] = shared_ptr<VirtualFont>(font);
        
        XmlTree doc(InputSource::getDataSource(ref));
        auto rootElement = doc.getChild("VirtualFont");
        
        for (auto &fontElement : rootElement.getChildren())
        {
            auto langList = getLanguageList(fontElement->getAttributeValue<string>("lang", ""));
            
            for (auto lang : langList)
            {
                for (auto &variantElement : fontElement->getChildren())
                {
                    if (variantElement->getTag() == "Group")
                    {
                        for (auto &refElement : variantElement->getChildren())
                        {
                            string ref = refElement->getValue();
                            
                            if (font->add(lang, getActualFont(ref, fontSize, useMipmap)))
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        string ref = variantElement->getValue();
                        font->add(lang, getActualFont(ref, fontSize, useMipmap));
                    }
                }
            }
        }
        
        return font;
    }
}

vector<string> FontManager::getLanguageList(const string &languages)
{
	return split(languages, ":");
}
