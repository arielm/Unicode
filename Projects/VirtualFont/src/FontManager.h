/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "TextItemizer.h"
#include "LayoutCache.h"

#include "chronotext/InputSource.h"

struct FontKey
{
    std::string uri;
    float baseSize;
    bool useMipmap;
    
    FontKey(const std::string &uri, float baseSize, bool useMipmap)
    :
    uri(uri),
    baseSize(baseSize),
    useMipmap(useMipmap)
    {}
    
    bool operator<(const FontKey &rhs) const
    {
        return tie(useMipmap, baseSize, uri) < tie(rhs.useMipmap, rhs.baseSize, rhs.uri);
    }
};

class FontManager
{
public:
    std::shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE ActualFont INSTANCES
    TextItemizer itemizer;
    LayoutCache layoutCache;
    
    FontManager();
    
    /*
     * THE GLOBAL-MAP, MAPPING BETWEEN:
     * - A STRING LIKE "sans-serif" AND SOME STYLE (E.G. VirtualFont::STYLE_BOLD)
     * - AND SOME FONT XML-DEFINITION LIKE "sans-serif_bold_ios.xml"
     */
    void loadGlobalMap(chr::InputSourceRef source);

    /*
     * SCENARIO 1:
     * - CALLER IS DEFINING A FONT-SIZE (baseSize != 0)
     * - IF A FONT-SIZE IS DEFINED AT THE GLOBAL-MAP LEVEL (base-size ATTRIBUTE), IT IS IGNORED
     * - MIPMAPS ARE NOT ALLOWED
     * - VirtualFont::setSize() IS NOT SUPPOSED TO BE USED DURING THE FONT'S LIFE-CYCLE
     *
     * SCENARIO 2:
     * - CALLER IS NOT DEFINING A FONT-SIZE (baseSize == 0)
     * - THE FONT-SIZE DEFINED AT THE GLOBAL-MAP LEVEL (base-size ATTRIBUTE) IS USED
     * - MIPMAPS ARE ALLOWED
     * - VirtualFont::setSize() SHALL BE USED DURING THE FONT'S LIFE-CYCLE
     *
     * FURTHER CALLS WILL BE CACHED.
     * THE RETURNED POINTER IS MANAGED BY FontManager AND WILL BE VALID AS LONG AS THE LATTER IS ALIVE
     */
    VirtualFont* getFont(const std::string &name, int style = VirtualFont::STYLE_PLAIN, float baseSize = 0);
    
    /*
     * LOWER-LEVEL METHOD, FOR ACCESSING A FONT DIRECTLY VIA ITS XML-DEFINITION
     *
     * FURTHER CALLS WILL BE CACHED.
     * THE RETURNED POINTER IS MANAGED BY FontManager AND WILL BE VALID AS LONG AS THE LATTER IS ALIVE
     */
    VirtualFont* getFont(chr::InputSourceRef source, float baseSize, bool useMipmap = false);
    
    void reload();
    void unload();
    void discardTextures();
    
protected:
    int platform;
    std::map<std::pair<std::string, int>, std::pair<std::string, float>> globalMap;
    
    std::map<FontKey, std::unique_ptr<VirtualFont>> virtualFonts;
    std::map<FontKey, std::unique_ptr<ActualFont>> actualFonts;

    ActualFont* getActualFont(const std::string &uri, float baseSize, bool useMipmap = false);

    static std::vector<std::string> splitLanguages(const std::string &languages);
    static int parseStyle(const std::string &style);
};
