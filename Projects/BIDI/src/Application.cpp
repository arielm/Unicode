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
 * RESULTS:
 * THEY SEEM COMPARABLE, AS LONG AS WE DON'T NECESSARY USE A
 * DEFAULT paraLevel OF "UBIDI_DEFAULT_LTR" IN ubidi_setPara()
 *
 *
 * TODO: ADD MORE TESTS
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
    Test::start("SIMPLE");
    Test::bidiMapnik("The title is مفتاح معايير الويب in Arabic.");
    Test::bidiAndroid("The title is مفتاح معايير الويب in Arabic.");

    Test::start("ADVANCED");
    Test::bidiMapnik("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", 1);
    Test::bidiAndroid("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", kBidi_RTL);
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
