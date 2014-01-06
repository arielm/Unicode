/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * SCRIPT DETECTION IMPLEMENTED USING:
 * http://source.icu-project.org/repos/icu/icu/trunk/source/extra/scrptrun
 *
 * THE NEXT STEP WOULD BE TO ASSOCIATE A LANGUAGE TO EACH OF THE SCRIPTS, AS DESCRIBED IN:
 * http://www.mail-archive.com/harfbuzz@lists.freedesktop.org/msg03220.html
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
    Test::run();
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
