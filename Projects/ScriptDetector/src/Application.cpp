/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * IMPLEMENTING icu/source/extra/scrptrun/srtest.cpp
 *
 *
 * TODO:
 *
 * 1) COMPILE ON ANDROID AND iOS:
 *    - CHECK IF THE DEFINES (SEE IN Prefix.pch) HELP ON ANDROID
 *
 * 2) USE std::string
 */

#include "cinder/app/AppNative.h"

#include "scrptrun.h"

using namespace std;
using namespace ci;
using namespace app;

UChar testChars[] =
{
    0x0020, 0x0946, 0x0939, 0x093F, 0x0928, 0x094D, 0x0926, 0x0940, 0x0020,
    0x0627, 0x0644, 0x0639, 0x0631, 0x0628, 0x064A, 0x0629, 0x0020,
    0x0420, 0x0443, 0x0441, 0x0441, 0x043A, 0x0438, 0x0439, 0x0020,
    'E', 'n', 'g', 'l', 'i', 's', 'h',  0x0020,
    0x6F22, 0x5B75, 0x3068, 0x3072, 0x3089, 0x304C, 0x306A, 0x3068,
    0x30AB, 0x30BF, 0x30AB, 0x30CA,
    0xD801, 0xDC00, 0xD801, 0xDC01, 0xD801, 0xDC02, 0xD801, 0xDC03
};

int32_t testLength = sizeof testChars / sizeof testChars[0];

class Application : public AppNative
{
public:
    void setup();
    void draw();
};

void Application::setup()
{
    ScriptRun scriptRun(testChars, 0, testLength);
    
    while (scriptRun.next())
    {
        int32_t     start = scriptRun.getScriptStart();
        int32_t     end   = scriptRun.getScriptEnd();
        UScriptCode code  = scriptRun.getScriptCode();
        
        printf("Script '%s' from %d to %d.\n", uscript_getName(code), start, end);
    }
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
