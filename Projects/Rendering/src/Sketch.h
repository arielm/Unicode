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
 * 1) ActualFont: ENHANCING getGlyph() AND createGlyph()
 *
 * 2) POSSIBILITY TO UNLOAD/RELOAD ActualFont INSTANCES:
 *    - UPON UNLOADING, THE MEMORY IS EMPTIED, BUT THE ActualFont INSTANCE
 *      AND ITS POINTERS (E.G. INSIDE Cluster INSTANCES) WILL BE PRESERVED
 *    - DURING THE TIME AN ActualFont IS UNLOADED:
 *      CREATING LAYOUTS OR DRAWING THEM WILL BE A NO-OP
 *    - PRESS "u" OR "r" ON THE DESKTOP TO UNLOAD/RELOAD
 *
 * 3) FontManager: MORE ROBUST PARSING OF VirtualFont
 */

/*
 * TODO:
 *
 * 0) "HIGHER-LEVEL" FONT-LOADING IN FontManager
 *
 * 0) ActualFont: POSSIBILITY TO LOAD FROM MEMORY...
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
 *
 * 2) ADD SCALE-FACTOR FOR ACTUAL-FONTS IN XML DEFINITION:
 *    - NECESSARY IN CASE WE NEED TO MATCH SIZES BETWEEN "Geeza Pro" (SMALLER?) AND "Arial" (BIGGER?)
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "FontManager.h"
#include "LayoutCache.h"

#include "cinder/Rand.h"

class Sketch : public chr::CinderSketch
{
public:
    FontManager fontManager;
    LayoutCache layoutCache;
    
    VirtualFont *font;
    std::vector<std::unique_ptr<LineLayout>> lineLayouts;
    
    std::vector<std::string> sentences;
    ci::Rand rnd;
    
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void draw();
    
    void drawLineLayout(LineLayout &layout, float y, float left, float right);
    static void drawHLine(float y);
    static std::string trimText(const std::string &text);
};
