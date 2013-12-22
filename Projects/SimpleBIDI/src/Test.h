/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * TESTING TWO METHODS FOR BIDI:
 *
 * bidiMapnik() IS USING THE FOLLOWING:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/src/text/itemizer.cpp#L90-131
 *
 * bidiAndroid() IS USING THE FOLLOWING:
 * https://github.com/android/platform_frameworks_base/blob/677726b376402937f53ddb192dc97078b92b7c9e/core/jni/android/graphics/TextLayoutCache.cpp#L356-485
 *
 *
 * RESULTS:
 *
 * THE 2 METHODS PRODUCE SIMILAR RESULTS:
 * AS LONG AS WE DON'T NECESSARLY USE A paraLevel
 * OF "UBIDI_DEFAULT_LTR" IN ubidi_setPara()
 *
 * THE CODE FROM THE ANDROID-PLATFORM IS SHOWING US THE WAY:
 * A POSSIBILITY TO USE kBidi_LTR (0) OR kBidi_RTL(1)
 * (NOTE THE LACK OF ICU CONSTANT FOR THESE VALUES)
 * BTW: USING "UBIDI_DEFAULT_RTL" WOULD NOT HAVE HELPED FOR TEST #4
 */

#pragma once

#include "unicode/unistr.h"
#include "unicode/uscript.h"
#include "unicode/ubidi.h"

#include <iostream>

enum
{
    kBidi_LTR = 0,
    kBidi_RTL = 1,
    kBidi_Default_LTR = 2,
    kBidi_Default_RTL = 3,
    kBidi_Force_LTR = 4,
    kBidi_Force_RTL = 5,
    
    kBidi_Mask = 0x7
};

enum
{
    kDirection_LTR = 0,
    kDirection_RTL = 1,
    
    kDirection_Mask = 0x1
};

#define SPIT_DETAILS

class Test
{
    std::ostream &output;
    
public:
    Test(std::ostream &output);
    
    void run();
    
    void spitComment(const std::string &text);
    void spitRun(const UnicodeString &text, UBiDiDirection direction, int32_t start, int32_t end);
    
    void bidiMapnik (const std::string &input, UBiDiLevel paraLevel = UBIDI_DEFAULT_LTR);
    void bidiAndroid(const std::string &input, int dirFlags = kBidi_Default_LTR);
};
