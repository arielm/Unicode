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
 * bidi1:
 * USING THE FOLLOWING:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/src/text/itemizer.cpp#L90-131
 *
 * bidi2:
 * USING THE FOLLOWING (TODO):
 * 
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

class Application : public AppNative
{
public:
    void setup();
    void draw();
};

void Application::setup()
{
    bidi1("The title is مفتاح معايير الويب in Arabic.");
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
