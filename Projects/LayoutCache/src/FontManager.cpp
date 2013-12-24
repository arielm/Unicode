#include "FontManager.h"

#include "cinder/app/App.h"
#include "cinder/Xml.h"

#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using namespace ci;
using namespace app;

const string REF_FILE = "file://";
const string REF_ASSETS = "assets://";

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

void FontManager::getFontTree(DataSourceRef source, float fontSize)
{
    XmlTree doc(source);
    auto rootElement = doc.getChild("FontTree");
    
    for (auto &fontElement : rootElement.getChildren())
    {
        
    }
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
