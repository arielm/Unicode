/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "FontManager.h"

#include "cinder/app/App.h"
#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace app;

const string REF_FILE = "file://";
const string REF_ASSETS = "assets://";
const string REF_RES = "res://";

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
        auto filePath = getFilePath(ref);
        
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
        
        XmlTree doc(getDataSource(ref));
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

fs::path FontManager::getFilePath(const std::string &ref)
{
    fs::path filePath;
    
    if (boost::starts_with(ref, REF_FILE))
    {
        filePath = ref.substr(REF_FILE.size());
    }
    else if (boost::starts_with(ref, REF_ASSETS))
    {
        filePath = getAssetPath(ref.substr(REF_ASSETS.size()));
    }
#if defined(CINDER_COCOA)
    else if (boost::starts_with(ref, REF_RES))
    {
        filePath = App::getResourcePath(ref.substr(REF_RES.size()));
    }
#endif
    
    if (fs::exists(filePath))
    {
        return filePath;
    }
    else
    {
        return "";
    }
}

DataSourceRef FontManager::getDataSource(const string &ref)
{
    if (boost::starts_with(ref, REF_FILE))
    {
        return DataSourcePath::create(ref.substr(REF_FILE.size()));
    }
    else if (boost::starts_with(ref, REF_ASSETS))
    {
        return DataSourcePath::create(getAssetPath(ref.substr(REF_FILE.size()))); // TODO: SPECIAL VERSION REQUIRED FOR ANDROID
    }
    else if (boost::starts_with(ref, REF_RES))
    {
        return loadResource(ref.substr(REF_RES.size())); // TODO: SPECIAL VERSION REQUIRED FOR WINDOWS
    }
    
    return DataSourceRef();
}
