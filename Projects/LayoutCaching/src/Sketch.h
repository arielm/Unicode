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
 * 1) LayoutCache IN PLACE:
 *    - ADAPTED TO TextLine
 *    - LRU CACHE STRATEGY BASED ON boost::bimaps
 *
 * 2) TESTED ON iOS AND ANDROID
 *
 * 3) ADAPTED TO new-chronotext-toolkit:
 *    NO MORE CRASH WHEN RETURNING TO APP ON XOOM 1 (ANDROID 3)
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
