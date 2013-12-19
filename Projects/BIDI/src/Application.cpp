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
 * bidiMapnik() IS USING THE FOLLOWING:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/src/text/itemizer.cpp#L90-131
 *
 * bidiAndroid() IS USING THE FOLLOWING:
 * https://github.com/android/platform_frameworks_base/blob/677726b376402937f53ddb192dc97078b92b7c9e/core/jni/android/graphics/TextLayoutCache.cpp#L356-485
 *
 *
 * RESULTS:
 * - THE 2 METHODS SEEM COMPARABLE, AS LONG AS WE DON'T NECESSARY USE
 *   A DEFAULT paraLevel OF "UBIDI_DEFAULT_LTR" IN ubidi_setPara()
 * - THE ANDROID-PLATFORM IS SHOWING US THE WAY:
 *   - THE POSSIBILITY TO USE kBidi_LTR (0) OR kBidi_RTL(1)
 *     (NOTE THE LACK OF ICU CONSTANT FOR THESE VALUES)
 *     - BTW, USING "UBIDI_DEFAULT_LTR" WOULD NOT HAVE HELPER FOR TEST #4
 *
 *
 * TODO:
 *
 * 1) MOVE ALL THE TESTS TO Test::run()
 *
 * 2) RENDER RUNS WITH HARFBUZZ
 *    - INSIGHT: HARFBUZZ-DIRECTION IS GOING TO BE DEFINED AT THE BIDI STAGE
 *      I.E. A BIDI CALL WITH "FORCE-LTR" OR "FORCE-RTL" WOULD NOT
 *      USE ICU-BIDI, SOLELY AFFECTING THE HARFBUZZ-DIRECTION FLAG
 */

#include "cinder/app/AppNative.h"

#include "Test.h"

using namespace std;
using namespace ci;
using namespace app;

class Application : public AppNative
{
public:
    void setup();
    void draw();
};

void Application::setup()
{
    Test::start("1");
    Test::bidiMapnik("The title is مفتاح معايير الويب in Arabic.");
    Test::bidiAndroid("The title is مفتاح معايير الويب in Arabic.");
    
    Test::start("2");
    Test::bidiMapnik ("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.");
    Test::bidiAndroid("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.");
    
    Test::start("3");
    Test::bidiMapnik ("The names of these states in Arabic are مصر,‎ البحرين and الكويت respectively.");
    Test::bidiAndroid("The names of these states in Arabic are مصر,‎ البحرين and الكويت respectively.");

    Test::start("4");
    Test::bidiMapnik ("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", kBidi_RTL);
    Test::bidiAndroid("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", kBidi_RTL);
    
    Test::start("5");
    Test::bidiMapnik ("The title says \"W3C, פעילות הבינאום\" in Hebrew.");
    Test::bidiAndroid("The title says \"W3C, פעילות הבינאום\" in Hebrew.");
    
    Test::start("6");
    Test::bidiMapnik ("one two ثلاثة four خمسة");
    Test::bidiAndroid("one two ثلاثة four خمسة");
    
    Test::start("7");
    Test::bidiMapnik ("one two ثلاثة 1234 خمسة");
    Test::bidiAndroid("one two ثلاثة 1234 خمسة");
    
    // --- JUST CHECKING IF DIACRITICS ARE PROPERLY HANDLED ---
    
    Test::start("10");
    Test::bidiMapnik ("וְהָהַר, מַהוּ לַזֵּה? – זֹאת הִיא הַשְּׁאֵלָה.", kBidi_RTL);
    Test::bidiAndroid("וְהָהַר, מַהוּ לַזֵּה? – זֹאת הִיא הַשְּׁאֵלָה.", kBidi_RTL);
   }

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
