/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * IMPLEMENTING icu/source/extra/scrptrun/srtest.cpp
 */

#include "cinder/app/AppNative.h"

#include "unicode/unistr.h"

#include "scrptrun.h"

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
    const string input = " ॆहिन्दी العربية Русский English 漢孵とひらがなとカタカナ𐐀𐐁𐐂𐐃";
    
    UnicodeString text = UnicodeString::fromUTF8(input);
    ScriptRun scriptRun(text.getBuffer(), text.length());
    
    while (scriptRun.next())
    {
        auto start = scriptRun.getScriptStart();
        auto end = scriptRun.getScriptEnd();
        auto code = scriptRun.getScriptCode();
        
        string tmp;
        text.tempSubString(start, end - start).toUTF8String(tmp);
        
        cout << "SCRIPT '" << uscript_getName(code) << "' FROM " << start << " TO " << end - 1 << endl;
        cout << tmp << endl << endl;
    }
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
