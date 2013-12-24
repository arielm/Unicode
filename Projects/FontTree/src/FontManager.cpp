#include "FontManager.h"

#include "cinder/app/App.h"
#include "cinder/Xml.h"

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace app;

const string REF_FILE = "file://";
const string REF_ASSETS = "assets://";

bool FontTree::add(const string &lang, YFont *font)
{
    if (font)
    {
        fontListMap[lang].push_back(font);
        return true;
    }

    return false;
}

FontList FontTree::getFontList(const string &lang)
{
    auto it = fontListMap.find(lang);
    
    if (it == fontListMap.end())
    {
        it = fontListMap.find("");
        
        if (it == fontListMap.end())
        {
            return FontList();
        }
    }
    
    return it->second;
}

FontManager::FontManager()
{
    ftHelper = make_shared<FreetypeHelper>();
}

YFont* FontManager::getCachedFont(const string &ref, float fontSize)
{
    auto it = fontMap.find(FontKey(ref, fontSize));
    
    if (it != fontMap.end())
    {
        return it->second.get();
    }
    else
    {
        auto filePath = getFilePath(ref);
        
        if (!filePath.empty())
        {
            YFont *font = new YFont(ftHelper, filePath, fontSize);
            fontMap[FontKey(ref, fontSize)] = shared_ptr<YFont>(font);
            
            return font;
        }
    }
    
    return NULL;
}

FontTree FontManager::loadFontTree(DataSourceRef source, float fontSize)
{
    FontTree tree;
    
    XmlTree doc(source);
    auto rootElement = doc.getChild("FontTree");
    
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
                        
                        if (tree.add(lang, getCachedFont(ref, fontSize)))
                        {
                            break;
                        }
                    }
                }
                else
                {
                    string ref = variantElement->getValue();
                    tree.add(lang, getCachedFont(ref, fontSize));
                }
            }
        }
    }
    
    return tree;
}

fs::path FontManager::getFilePath(const std::string &ref)
{
    fs::path filePath;
    
    if (boost::starts_with(ref, REF_ASSETS))
    {
        filePath = getAssetPath(ref.substr(REF_ASSETS.size()));
    }
    if (boost::starts_with(ref, REF_FILE))
    {
        filePath = ref.substr(REF_FILE.size());
    }
    
    if (fs::exists(filePath))
    {
        return filePath;
    }
    else
    {
        return "";
    }
}

vector<string> FontManager::getLanguageList(const string &languages)
{
	vector<string> result;
	boost::algorithm::split(result, languages, boost::is_any_of(":"));
	return result;
}
