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
 */

/*
 * - PRESS U TO CALL FontManager::unload()
 * - PRESS R TO CALL FontManager::reload()
 * - PRESS SPACE TO SHUFFLE THE LINES
 * - PRESS SPACE TO TOGGLE SIZE OSCILLATION
 */

/*
 * TODO:
 *
 * 0) THE USAGE OF METRICS NEED TO BE REVISITED:
 *    - IT WILL NOT BE POSSIBLE TO GET ONE STRIKETHROUGH-OFFSET FOR A
 *      WHOLE LineLayout AND TO USE IT AS AN OFFSET PRIOR TO RENDERING
 *    - IDEM FOR ASCENT AND DESCENT...
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
