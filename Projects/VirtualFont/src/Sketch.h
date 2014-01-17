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
 *
 * 4) FontManager, VirtualFont AND ActualFont:
 *    - NOT PASSING padding AS ARGUMENT ANYMORE
 *      INSTEAD, IT IS "AUTOMATICALLY" DEFINED INSIDE ActualFont CONSTRUCTOR
 *    - useMipmap IS NOW PREDEFINED TO false BY DEFAULT
 *      - CONSISTENT WITH XFont
 *    - FONT-SIZE IS NOW SET TO baseSize INSIDE VirtualFont CONSTRUCTOR
 *      - SIMPLIFIES THE WORKFLOW WITH "NON MIPMAP" FONTS
 *        I.E. NO NEED TO CALL VirtualFont::setSize() AFTER THE FONT-CREATION...
 *
 * 5) "HIGHER-LEVEL" FONT-LOADING IN FontManager:
 *     - E.G. FontManager::load("sans-serif", VirtualFont::STYLE_BOLD, 27)
 *     - THANKS TO A FONT-DEFINITION XML FILE ALLOWING TO MAP BETWEEN NAME/STYLE AND URI
 *
 * 6) LayoutManager IS NOW DEFINED WITHIN FontManager:
 *    - SIMILARELY AS TextItemizer
 *    - WILL SIMPLIFY USAGE, BY REQUIRING ONLY ONE "PSEUDO SINGLETON" (I.E. FontManager)
 *
 * 7) POSSIBILITY TO LOAD ActualFont FROM MEMORY-BUFFER:
 *    - NECESSARY FOR ANDROID ASSETS AND WINDOWS EMBEDDED RESOURCES
 *    - SEE ActualFont::Descriptor
 *
 * 8) ActualFont: POSSIBILITY TO DEFINE FACE-INDEX AT THE XML LEVEL
 *    - VIA THE face-index ATTRIBUTE
 *    - SEE ActualFont::Descriptor
 *
 * 9) ActualFont: POSSIBILITY TO DEFINE SCALE AT THE XML LEVEL
 *    - VIA THE scale ATTRIBUTE
 *    - SEE ActualFont::Descriptor
 *    - NECESSARY FOR FONT-FALLBACK IN ORDER TO MATCH SIZE BETWEEN NON PROPORTIONALLY SCALED FONTS
 */

/*
 * TODO:
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
    
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void draw();
    
    void drawLineLayout(LineLayout &layout, float y, float left, float right);
    static void drawHLine(float y);
    static std::string trimText(const std::string &text);
};
