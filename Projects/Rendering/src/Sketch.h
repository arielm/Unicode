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
 * 4) FIXING REGRESSION (INTRODUCED IN COMMIT 604c7ab) AFECTING (AT LEAST) HEBREW AND THAI
 *
 * 5) "COMPOSITE METRICS" FOR VirtualFont GIVEN A LineLayout:
 *    - getHeight() WILL RETURN THE MAXIMUM HEIGHT
 *    - getAscent() WILL RETURN THE MAXIMUM ASCENT
 *    - getDescent() WILL RETURN THE MAXIMUM DESCENT
 *    - getMiddleLine() WILL RETURN HALF OF getAscent() - getDescent():
 *      THIS IS LESS IDEAL THAN USING THE STRIKETHROUGH-OFFSET,
 *      BUT WE HAVE NO CHOICE IN THE CASE OF A "COMPOSITE" LineLayout
 *
 * 6) ActualFont::getFullName()
 */

/*
 * - PRESS U TO CALL FontManager::unload()
 * - PRESS R TO CALL FontManager::reload()
 * - PRESS ENTER TO SHUFFLE THE LINES
 * - PRESS SPACE TO TOGGLE SIZE OSCILLATION
 * - PRESS T, M OR P TO SWITCH BETWEEN TOP, MIDDLE OR BOTTOM ALIGNMENT
 */

/*
 * TODO:
 *
 * 0) VirtualFont::getFullName()
 *    - PREREQUISITES:
 *      - PARSING name AND style ATTRIBUTES IN VirtualFont ELEMENT
 *        IN CASE A VirtualFont IS INSTANCIATED "DIRECTLY" VIA InputSource
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

    void drawLineLayout(LineLayout &layout, float y, float left, float right);
    static void drawHLine(float y);
    
    void shuffleLines();
    static std::string trimText(const std::string &text);
};
