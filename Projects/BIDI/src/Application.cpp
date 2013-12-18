/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * USING BIDI TEXT EXAMPLES FROM:
 * http://people.w3.org/rishida/scripts/bidi/
 *
 *
 * bidi1() IS USING THE FOLLOWING:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/src/text/itemizer.cpp#L90-131
 *
 * bidi2() IS USING THE FOLLOWING:
 * https://github.com/android/platform_frameworks_base/blob/677726b376402937f53ddb192dc97078b92b7c9e/core/jni/android/graphics/TextLayoutCache.cpp#L356-485
 *
 * RESULTS ARE COMPARABLE...
 *
 *
 * TODO: TEST MORE EXAMPLES
 */

#include "cinder/app/AppNative.h"

#include "unicode/unistr.h"
#include "unicode/uscript.h"
#include "unicode/ubidi.h"

using namespace std;
using namespace ci;
using namespace app;

static void spitRun(const UnicodeString &text, UBiDiDirection direction, int32_t start, int32_t end)
{
    std::string tmp;
    text.tempSubString(start, end - start).toUTF8String(tmp);

    cout << ((direction == UBIDI_RTL) ? "RTL " : "") << "[" << start << " | " << end << "]" << endl;
    cout << tmp << endl << endl;
}

static void bidi1(const string &input)
{
    auto text = UnicodeString::fromUTF8(input);
    int32_t start = 0;
    int32_t end = text.length();
    
    // ---
    
    UErrorCode error = U_ZERO_ERROR;
    int32_t length = end - start;
    UBiDi *bidi = ubidi_openSized(length, 0, &error);
    
    if (!bidi || U_FAILURE(error))
    {
        cout << "Failed to create bidi object: " << u_errorName(error) << endl;
        throw;
    }
    
    ubidi_setPara(bidi, text.getBuffer() + start, length, UBIDI_DEFAULT_LTR, 0, &error);
    
    if (U_SUCCESS(error))
    {
        UBiDiDirection direction = ubidi_getDirection(bidi);
        if (direction != UBIDI_MIXED)
        {
            spitRun(text, direction, start, end);
        }
        else
        {
            // mixed-directional
            int32_t count = ubidi_countRuns(bidi, &error);
            
            if (U_SUCCESS(error))
            {
                for (int i=0; i<count; ++i)
                {
                    int32_t length;
                    int32_t run_start;
                    direction = ubidi_getVisualRun(bidi, i, &run_start, &length);
                    run_start += start; //Add offset to compensate offset in setPara
                    spitRun(text, direction, run_start, run_start + length);
                }
            }
        }
    }
    else
    {
        cout << "ICU error: " << u_errorName(error) << endl;
        throw;
    }
    
    ubidi_close(bidi);
}

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

static void bidi2(const string &input)
{
    auto text = UnicodeString::fromUTF8(input);
    
    const UChar *chars = text.getBuffer();
    size_t start = 0;
    size_t count = text.length();
    size_t contextCount = count; // ???
    
    int dirFlags = kBidi_Default_LTR;
    
    // ---
    
    UBiDiLevel bidiReq = 0;
    bool forceLTR = false;
    bool forceRTL = false;
    
    switch (dirFlags & kBidi_Mask)
    {
        case kBidi_LTR: bidiReq = 0; break; // no ICU constant, canonical LTR level
        case kBidi_RTL: bidiReq = 1; break; // no ICU constant, canonical RTL level
        case kBidi_Default_LTR: bidiReq = UBIDI_DEFAULT_LTR; break;
        case kBidi_Default_RTL: bidiReq = UBIDI_DEFAULT_RTL; break;
        case kBidi_Force_LTR: forceLTR = true; break; // every char is LTR
        case kBidi_Force_RTL: forceRTL = true; break; // every char is RTL
    }
    
    bool useSingleRun = false;
    bool isRTL = forceRTL;
    
    if (forceLTR || forceRTL)
    {
        useSingleRun = true;
    }
    else
    {
        UBiDi* bidi = ubidi_open();
        
        if (bidi)
        {
            UErrorCode status = U_ZERO_ERROR;
            ubidi_setPara(bidi, chars, contextCount, bidiReq, NULL, &status);
            
            if (U_SUCCESS(status))
            {
                int paraDir = ubidi_getParaLevel(bidi) & kDirection_Mask; // 0 if ltr, 1 if rtl
                ssize_t rc = ubidi_countRuns(bidi, &status);
                
                if (U_SUCCESS(status) && rc == 1)
                {
                    // Normal case: one run, status is ok
                    isRTL = (paraDir == 1);
                    useSingleRun = true;
                }
                else if (!U_SUCCESS(status) || rc < 1)
                {
                    // ALOGW("Need to force to single run -- string = '%s', status = %d, rc = %d", String8(chars + start, count).string(), status, int(rc));
                    isRTL = (paraDir == 1);
                    useSingleRun = true;
                }
                else
                {
                    int32_t end = start + count;
                    
                    for (size_t i = 0; i < size_t(rc); ++i)
                    {
                        int32_t startRun = -1;
                        int32_t lengthRun = -1;
                        UBiDiDirection runDir = ubidi_getVisualRun(bidi, i, &startRun, &lengthRun);
                        
                        if (startRun == -1 || lengthRun == -1)
                        {
                            // Something went wrong when getting the visual run, need to clear
                            // ALOGW("Visual run is not valid");
//                            outGlyphs->clear();
//                            outAdvances->clear();
//                            outPos->clear();
//                            *outTotalAdvance = 0;
                            isRTL = (paraDir == 1);
                            useSingleRun = true;
                            break;
                        }
                        
                        if (startRun >= end)
                        {
                            continue;
                        }
                        
                        int32_t endRun = startRun + lengthRun;
                        if (endRun <= int32_t(start))
                        {
                            continue;
                        }
                        if (startRun < int32_t(start))
                        {
                            startRun = int32_t(start);
                        }
                        if (endRun > end)
                        {
                            endRun = end;
                        }
                        
                        lengthRun = endRun - startRun;
                        isRTL = (runDir == UBIDI_RTL);
                        
//                        computeRunValues(paint, chars, startRun, lengthRun, contextCount, isRTL, outAdvances, outTotalAdvance, outBounds, outGlyphs, outPos);
                        spitRun(text, isRTL ? UBIDI_RTL : UBIDI_LTR, startRun, startRun + lengthRun);
                    }
                }
            }
            else
            {
                // ALOGW("Cannot set Para");
                useSingleRun = true;
                isRTL = (bidiReq = 1) || (bidiReq = UBIDI_DEFAULT_RTL);
            }
            ubidi_close(bidi);
        }
        else
        {
            // ALOGW("Cannot ubidi_open()");
            useSingleRun = true;
            isRTL = (bidiReq = 1) || (bidiReq = UBIDI_DEFAULT_RTL);
        }
    }
    
    // Default single run case
    if (useSingleRun)
    {
//        computeRunValues(paint, chars, start, count, contextCount, isRTL, outAdvances, outTotalAdvance, outBounds, outGlyphs, outPos);
        spitRun(text, isRTL ? UBIDI_RTL : UBIDI_LTR, start, start + count);
    }
}

class Application : public AppNative
{
public:
    void setup();
    void draw();
};

void Application::setup()
{
//  bidi1("The title is مفتاح معايير الويب in Arabic.");
    bidi2("The title is مفتاح معايير الويب in Arabic.");
    bidi2("foo bar");
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
