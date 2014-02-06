/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * DONE:
 *
 * 1) MOVING LangHelper TO FontManager AND PROVIDING PROXY TO LayoutCache IN VirtualFont
 *
 * 2) VirtualFont: ADDING MEANS OF OBTAINING ActualFont::Metrics (E.G. FROM A GIVEN Cluster)
 *
 * 3) TextItemizer::processLine():
 *    - DIRECTION IS NOW UNDEFINED BY DEFAULT:
 *      - IN THIS CASE, THE "PARAGRAPH-LEVEL" WILL BE DETERMINED FROM THE TEXT (VIA BIDI)
 *        AND THE RESULTING LineLayout WILL RECEIVE AN overallDirection VALUE
 *    - LIKEWISE, IF LANGUAGE IS UNDEFINED:
 *      - THE RESULTING LineLayout WILL RECEIVE A langHint VALUE BASED
 *        ON THE LANGUAGE DETECTED FOR THE FIRST RUN
 *
 * 4) FIXING REGRESSION (INTRODUCED IN COMMIT 604c7ab) AFFECTING (AT LEAST) HEBREW AND THAI
 *
 * 5) "COMPOSITE METRICS" FOR VirtualFont GIVEN A LineLayout:
 *    - getHeight() WILL RETURN THE MAXIMUM HEIGHT
 *    - getAscent() WILL RETURN THE MAXIMUM ASCENT
 *    - getDescent() WILL RETURN THE MAXIMUM DESCENT
 *    - getMiddleLine() WILL RETURN HALF OF getAscent() - getDescent():
 *      THIS IS LESS IDEAL THAN USING THE STRIKETHROUGH-OFFSET,
 *      BUT WE HAVE NO CHOICE IN THE CASE OF A "COMPOSITE" LineLayout
 *
 * 6) API ADDITIONS / IMPROVEMENTS:
 *    - ActualFont::getFullName()
 *    - VirtualFont::getCachedLineLayout()
 *
 * 7) SETTING DEFAULT COLOR FOR VirtualFont
 *
 * 8) FontManager::getFont()
 *    - FASTER LOOKUPS VIA THE shortcuts MAP
 *    - BASIC SYSTEM FOR HANDLING UNDEFINED FONT NAMES AND STYLES:
 *      - getFont() IS NOW SUPPOSED TO THROW ONLY UPON MALFORMED XML
 *        OR WHEN A FONT NAME/STYLE COMBINATION DOES NOT EXIST
 *        AND NO "DEFAULT FONT" HAS BEEN DEFINED
 *
 * 9) PREPARING THE TERRAIN FOR OPTIMIZED DRAWING:
 *    - DRAWING THE HORIZONTAL-LINES AND THE TEXT SEPARATELY
 *    - ONE SINGLE VirtualFont::begin() AND VirtualFont::end() PAIRS ARE USED
 *      - THE NEXT STEP WILL BE TO USE A SYSTEM FOR GROUPING SIMILAR GLYPH TEXTURES...
 *    - USING glDrawArrays WITH INTERLEAVED "POSITIONS + TEXTURE-COORDS"
 *
 * 10) FontManager: POSSIBILITY TO QUERY "TEXTURE MEMORY USAGE"
 *
 * 11) LayoutCache: POSSIBILITY TO QUERY MEMORY USAGE AND TO CHANGE CAPACITY
 *
 * 12) FINE-TUNING:
 *     - FontManager::getCachedFont() METHODS ARE NOW RETURNING A shared_ptr<VirtualFont>:
 *       - AVOID RISK OF UN-NECESSARY COPY
 *       - NECESSARY IN ORDER TO PROPERLY REMOVE VirtualFont INSTANCES (TBD)
 *     - FontManager::getCachedLineLayout() AND LayoutCache::getLineLayout() ARE NOW RETURNING A shared_ptr<LineLayout>:
 *       - AVOID RISK OF UN-NECESSARY COPY
 *       - THE CACHE WAS ALREADY USING A shared_ptr INTERNALLY
 *       - MORE ADAPTED TO THE NOTION OF CACHE, AND NOW CONSISTENT WITH FontManager::getCachedFont()
 *     - FINALLY FOUND A WAY TO USE PROTECTED CONSTRUCTORS FOR
 *       VirtualFont AND ActualFont (WHICH SHOULD BE CREATED ONLY VIA FontManager)
 *     - FontManager::loadGlobalMap() CAN ONLY BE CALLED ONCE (FURTHER ATTEMPTS WILL THROW...)
 *
 * 13) REDESIGN OF THE ActualFont LIFE-CYCLE:
 *     - FontManager::reload() HAS BEEN CANCELLED, SINCE FONT RESOURCES (HARFBUZZ AND FREETYPE RELATED)
 *       ARE NOW RELOADED ONLY WHEN NECESSARY (EITHER UPON LineLayout RENDERING OR CREATION)
 *     - PREVIOUSLY: AFTER CALLING FontManager::unload(), LineLayout RENDERING OR CREATION
 *       WAS SIMPLY NOT TAKING PLACE (I.E. UNTIL reload() IS CALLED),
 *       BUT THE SYSTEM CAN'T BE ADAPTED TO LAZY RELOADING
 */

/*
 * ON THE DESKTOP:
 * - PRESS U TO CALL FontManager::unload()
 * - PRESS ENTER TO SHUFFLE THE LINES
 * - PRESS SPACE TO TOGGLE SIZE OSCILLATION
 * - PRESS T, M OR P TO SWITCH BETWEEN TOP, MIDDLE OR BOTTOM ALIGNMENTS
 * - PRESS X TO PRINT TEXTURE MEMORY USAGE
 */

/*
 * TODO:
 *
 * 0) ActualFont INSTANCES SHOULD BE LAZILY LOADED FROM THE BEGINNING:
 *    - CURRENTLY: THEY ARE FULLY LOADED FROM THE BEGINNING, IN ORDER TO
 *      EXCLUDE FROM THE VirtualFont'S FontSet ANY UNLOADABLE ActualFont
 *
 * 0) FontManager::getFont()
 *    - XML FORMAT:
 *      - "DEFAULT FONT" SYSTEM SHOULD BE DEFINED AT THE XML-LEVEL
 *      - POSSIBILITY TO DEFINE ALIASES, E.G.
 *        - "Arial" COULD BE AN ALIAS FOR "sans-serif"
 *        - "Times" COULD BE AN ALIAS FOR "serif"
 *
 * 0) VirtualFont::getFullName()
 *    - DEPENDING ON HOW THE FONT WAS INITIATED, WILL USE EITHER:
 *      - THE name AND style ATTRIBUTES
 *      - THE INPUT-SOURCE URI
 *
 * 0) InputSource: "internal" PROTOCOL:
 *    - TO BE USED FOR "LOADING FONTS COPIED FROM ASSETS" ON ANDROID
 *    - FIND-OUT HOW TO COPY FROM ASSETS TO "INTERNAL FOLDER" ON ANDROID
 *
 * 0) ADJUST FONTS:
 *    - iOS:
 *      - "Geeza Pro" SHOULD BE REPLACED (ARABIC GLYPHS ARE NOT JOINED)
 *    - ANDROID 3:
 *      - HEBREW FONT IS MISSING
 *      - ARABIC FONT IS PROBABLY OUTDATED
 *      - THAI FONT IS BUGGY...
 *
 * 0) ADVANCED GLYPH RENDERING:
 *    - BATCHING ("TEXTURE BUCKET"):
 *      - USING A MAP, WITH AN ENTRY FOR EACH TEXTURE:
 *        - EACH ENTRY CONTAINING A VECTOR OF FLOATS FILLED-WITH:
 *          - INDICES
 *          - INTERLEAVED POSITIONS, TEXTURE-COORDS AND COLORS
 *    - TRANSFORMING VERTICES VIA FontMatrix LIKE IN XFont
 *    - "BEGIN / END" MODES:
 *      - "DIRECT"
 *      - "TEXTURE BUCKET"
 *
 * 0) TEXTURE-ATLASES:
 *    - SIMILAR TO THE SYSTEM USED IN XFont, BUT WITH N TEXTURES
 *    - WE NEED A SYSTEM FOR USERS TO DEFINE WHICH GLYPHS ARE CACHED
 *      - BECAUSE OF MIPMAPPING, WE CAN'T ADD GLYPHS ON-THE-FLY
 *      - THERE SHOULD BE A WAY TO ADD/REMOVE GROUPS OF GLYPHS, E.G. PER LANGUAGE
 *
 * 0) FontManager:
 *    - WE NEED A KIND OF "GLOBAL TEXTURE STORE" FOR STANDALONE-TEXTURES AND ATLASES:
 *      - LRU STRATEGY FOR STANDALONE-TEXTURES?
 *      - AUTOMATIC REMOVAL OF STANDALONE-TEXTURE WHENEVER AN ATLAS IS STORING THE ASSOCIATED GLYPH'S TEXTURE
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "FontManager.h"

#include "cinder/Rand.h"

class Sketch : public chr::CinderSketch
{
public:
    FontManager fontManager;
    
    std::vector<std::string> sentences;
    ci::Rand rnd;
    std::vector<std::string> lines;

    float fontSize;
    VirtualFont::Alignment align;
    bool oscillate;
    
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void update();
    void draw();

    void drawTextLine(VirtualFont &font, const std::string &text, float y, float left, float right);
    static void drawHLines(int count, float top, float spacing);
    static void drawHLine(float y);
    
    void shuffleLines();
    static std::string trimText(const std::string &text);
};
