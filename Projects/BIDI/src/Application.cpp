/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * TODO:
 *
 * 1) RENDER RUNS WITH HARFBUZZ
 *    - INSIGHT: HARFBUZZ-DIRECTION IS GOING TO BE DEFINED AT THE BIDI STAGE
 *      I.E. A BIDI CALL WITH "FORCE-LTR" OR "FORCE-RTL" WOULD NOT
 *      USE ICU-BIDI, SOLELY AFFECTING THE HARFBUZZ-DIRECTION FLAG
 */

#include "cinder/app/AppNative.h"

#include "Test.h"

#include <hb.h>

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
    Test(console()).run(); // TESTED ON OSX, iOS AND ANDROID
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
