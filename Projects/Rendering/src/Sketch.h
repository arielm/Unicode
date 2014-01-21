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
