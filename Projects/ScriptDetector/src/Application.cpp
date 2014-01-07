/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * DONE:
 *
 * 1) SCRIPT DETECTION IMPLEMENTED USING:
 *    http://source.icu-project.org/repos/icu/icu/trunk/source/extra/scrptrun
 *
 * 2) LANGUAGE DETECTION, AS DESCRIBED IN:
 *    http://www.mail-archive.com/harfbuzz@lists.freedesktop.org/msg03220.html
 *
 *
 * TODO:
 *
 * BIDI ITEMIZATION SHOULD TAKE PLACE,
 * THEN THE SCRIPT/LANGUAGE AND BIDI ITEMS SHOULD BE "MIXED",
 * AS DESCRIBED IN http://www.mail-archive.com/harfbuzz@lists.freedesktop.org/msg03190.html
 * AND AS IMPLEMENTED IN THE MAPNIK PROJECT
 */

#include "cinder/app/AppNative.h"

#include "LanguageHelper.h"

#include "unicode/unistr.h"
#include "scrptrun.h"

using namespace std;
using namespace ci;
using namespace app;

class Application : public AppNative
{
    LanguageHelper languageHelper;
    
public:
    void setup();
    void draw();
    
    void detect(const string &itemText, const string &itemLanguage = "");
};

void Application::setup()
{
    detect(" ॆहिन्दी العربية Русский English 漢孵とひらがなとカタカナ𐐀𐐁𐐂𐐃");
    detect("The title says \"W3C פעילות הבינאום,\u200f\" in Hebrew.");
    detect("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.");
    detect("ユニコードは、すべての文字に固有の番号を付与します", "ja"); // IF "ja" WERE UNDEFINED, THE HAN CHARACTERS WOULD HAVE BEEN DETECTED AS "zh-cn"
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

void Application::detect(const string &itemText, const string &itemLanguage)
{
    UnicodeString text = UnicodeString::fromUTF8(itemText);
    ScriptRun scriptRun(text.getBuffer(), text.length());
    
    while (scriptRun.next())
    {
        auto start = scriptRun.getScriptStart();
        auto end = scriptRun.getScriptEnd();
        auto code = scriptRun.getScriptCode();
        
        string tmp;
        text.tempSubString(start, end - start).toUTF8String(tmp);
        
        auto language = languageHelper.detectLanguage(code, itemLanguage);
        
        cout << "SCRIPT '" << uscript_getName(code) << "' | LANG: '" << language << "' | FROM " << start << " TO " << end - 1 << std::endl;
        cout << tmp << std::endl << endl;
    }
    
    cout << endl;
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
