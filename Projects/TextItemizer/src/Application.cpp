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
 * 1) INCLUDE WHAT'S NECESSARY FOR BIDI...
 */

#include "cinder/app/AppNative.h"

#include <unicode/unistr.h>
#include <unicode/uscript.h>
#include <unicode/ubidi.h>

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
    string foo = "привет";
    UnicodeString uFoo = UnicodeString::fromUTF8(foo);
    
    UnicodeString uBar("לְהַגִּיד בַּבֹּקֶר חַסְדֶּךָ וֶאֱמוּנָתְךָ בַּלֵּילוֹת");
    
    string bar;
    uBar.toUTF8String(bar);
    
    cout << bar << endl;
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
