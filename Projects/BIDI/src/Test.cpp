#include "Test.h"

using namespace std;

Test::Test(ostream &output)
:
output(output)
{}

void Test::run()
{
    /*
     * USING BIDI TEXT EXAMPLES FROM:
     * http://people.w3.org/rishida/scripts/bidi/
     */
    
    spitComment("1");
    bidiMapnik("The title is مفتاح معايير الويب in Arabic.");
    bidiAndroid("The title is مفتاح معايير الويب in Arabic.");
    
    spitComment("2");
    bidiMapnik ("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.");
    bidiAndroid("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.");
    
    spitComment("3");
    bidiMapnik ("The names of these states in Arabic are مصر,‎ البحرين and الكويت respectively.");
    bidiAndroid("The names of these states in Arabic are مصر,‎ البحرين and الكويت respectively.");
    
    spitComment("4");
    bidiMapnik ("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", kBidi_RTL);
    bidiAndroid("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", kBidi_RTL);
    
    spitComment("5");
    bidiMapnik ("The title says \"W3C, פעילות הבינאום\" in Hebrew.");
    bidiAndroid("The title says \"W3C, פעילות הבינאום\" in Hebrew.");
    
    spitComment("6");
    bidiMapnik ("one two ثلاثة four خمسة");
    bidiAndroid("one two ثلاثة four خمسة");
    
    spitComment("7");
    bidiMapnik ("one two ثلاثة 1234 خمسة");
    bidiAndroid("one two ثلاثة 1234 خمسة");
    
    /*
     * JUST CHECKING IF DIACRITICS ARE PROPERLY HANDLED
     */
    
    spitComment("10");
    bidiMapnik ("וְהָהַר, מַהוּ לַזֵּה? – זֹאת הִיא הַשְּׁאֵלָה.", kBidi_RTL);
    bidiAndroid("וְהָהַר, מַהוּ לַזֵּה? – זֹאת הִיא הַשְּׁאֵלָה.", kBidi_RTL);
}

void Test::spitComment(const string &text)
{
    output << "-------------------- " << text << " --------------------" << endl << endl;
}

/*
 * EACH RUN MUST BE ON ITS OWN LINE BECAUSE IN XCode'S CONSOLE IS RE-BIDIZING THE OUTPUT
 */
void Test::spitRun(const UnicodeString &text, UBiDiDirection direction, int32_t start, int32_t end)
{
    std::string tmp;
    text.tempSubString(start, end - start).toUTF8String(tmp);

#ifdef SPIT_DETAILS
    output << ((direction == UBIDI_RTL) ? "RTL " : "") << "[" << start << " | " << end << "]" << endl;
#endif
    
    output << tmp << endl;
    
#ifdef SPIT_DETAILS
    output << endl;
#endif
}

// ---------------------------------------- MAPNIK ----------------------------------------

void Test::bidiMapnik(const string &input, UBiDiLevel paraLevel)
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
        output << "Failed to create bidi object: " << u_errorName(error) << endl;
        throw;
    }
    
    ubidi_setPara(bidi, text.getBuffer() + start, length, paraLevel, 0, &error);
    
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
                for (int i = 0; i < count; ++i)
                {
                    int32_t length;
                    int32_t run_start;
                    direction = ubidi_getVisualRun(bidi, i, &run_start, &length);
                    run_start += start; // Add offset to compensate offset in setPara
                    spitRun(text, direction, run_start, run_start + length);
                }
            }
        }
    }
    else
    {
        output << "ICU error: " << u_errorName(error) << endl;
        throw;
    }
    
    ubidi_close(bidi);
    
    // ---
    
    output << endl;
}

// ---------------------------------------- ANDROID ----------------------------------------

void Test::bidiAndroid(const string &input, int dirFlags)
{
    auto text = UnicodeString::fromUTF8(input);
    
    const UChar *chars = text.getBuffer();
    size_t start = 0;
    size_t count = text.length();
    size_t contextCount = count; // XXX: WHAT'S THE PURPOSE OF contextCount?
    
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
        UBiDi *bidi = ubidi_open();
        
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
//                          outGlyphs->clear();
//                          outAdvances->clear();
//                          outPos->clear();
//                          *outTotalAdvance = 0;
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
                        
//                      computeRunValues(paint, chars, startRun, lengthRun, contextCount, isRTL, outAdvances, outTotalAdvance, outBounds, outGlyphs, outPos);
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
//      computeRunValues(paint, chars, start, count, contextCount, isRTL, outAdvances, outTotalAdvance, outBounds, outGlyphs, outPos);
        spitRun(text, isRTL ? UBIDI_RTL : UBIDI_LTR, start, start + count);
    }
    
    // ---
    
    output << endl;
}
