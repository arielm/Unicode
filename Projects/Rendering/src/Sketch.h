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
 * 1) MOVING LangHelper TO FontManager
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
 * 7) CACHED RESULTS ARE NOW MORE FLEXIBLE:
 *    - LayoutCache::getLineLayout() IS NOW RETURNING A CONST REFERENCE:
 *      - ALLOWS FOR EASY COPY OF THE RETURNED LineLayout
 *    - FontManager::getFont() METHODS ARE NOW RETURNING A REFERENCE:
 *      - ALLOWS FOR EASY COPY OF THE RETURNED VirtualFont (WHICH IS A RELATIVELY LIGHTWEIGHT OBJECT BTW)
 *    - BEWARE:
 *      - UN-NECESSARY COPIES CAN OCCUR EASILY
 *      - RECOMMENDED USAGE TO AVOID COPIES:
 *        - auto &layout = layoutCache.getLineLayout(...);
 *        - auto &font = fontManager.getFont(...);
 */

/*
 * - PRESS U TO CALL FontManager::unload()
 * - PRESS R TO CALL FontManager::reload()
 * - PRESS ENTER TO SHUFFLE THE LINES
 * - PRESS SPACE TO TOGGLE SIZE OSCILLATION
 * - PRESS T, M OR P TO SWITCH BETWEEN TOP, MIDDLE OR BOTTOM ALIGNMENTS
 */

/*
 * TODO:
 *
 * 0) FontManager::getFont()
 *    - PROVIDE FASTER LOOKUPS (BASED on name AND style)
 *    - XML FORMAT:
 *      - POSSIBILITY TO DEFINE ALIASES, E.G.
 *        - "Arial" COULD BE AN ALIAS FOR "sans-serif"
 *        - "Times" COULD BE AN ALIAS FOR "serif"
 *      - POSSIBILITY TO DEFINE A "GLOBAL FALLBACK" FONT, E.G. "sans-serif"
 *      - THEN: getFont() WOULD CRASH ONLY WHEN NO ALIASES NOR "GLOBAL FALLBACK" FONT ARE DEFINED
 *
 * 0) VirtualFont::getFullName()
 *    - DEPENDING ON HOW THE FONT WAS INITIATED, WILL USE EITHER:
 *      - THE name AND style ATTRIBUTES
 *      - THE INPUT-SOURCE URI
 *
 * 0) InputSource: "internal" PROTOCOL:
 *    - TO BE USED FOR "LOADING FONTS COPIED FROM ASSETS" ON ANDROID
 *    - SEE HOW TO COPY FROM ASSETS
 *
 * 1) ADJUST FONTS:
 *    - iOS:
 *      - "Geeza Pro" SHOULD BE REPLACED (ARABIC GLYPHS ARE NOT JOINED)
 *    - ANDROID:
 *      - HEBREW FONT IS MISSING
 *      - ARABIC FONT IS PROBABLY OUTDATED
 *      - THAI FONT IS BUGGY...
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "FontManager.h"

#include "cinder/Rand.h"

class Sketch : public chr::CinderSketch
{
public:
    FontManager fontManager;
    
    VirtualFont *font;
    std::vector<std::unique_ptr<LineLayout>> lineLayouts;
    
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

    void drawLineLayout(const LineLayout &layout, float y, float left, float right);
    static void drawHLine(float y);
    
    void shuffleLines();
    static std::string trimText(const std::string &text);
};
