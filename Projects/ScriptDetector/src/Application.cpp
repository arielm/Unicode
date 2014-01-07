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
 *
 * THEN, BIDI ITEMIZATION SHOULD TAKE PLACE AND THE "ITEMS SHOULD BE MIXED",
 * AS DESCRIBED IN http://www.mail-archive.com/harfbuzz@lists.freedesktop.org/msg03190.html
 * AND AS IMPLEMENTED IN THE MAPNIK PROJECT
 */

#include "cinder/app/AppNative.h"

#include "LanguageHelper.h"
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
    LanguageHelper languageHelper;
    
    std::cout << languageHelper.includesScript("ja", HB_SCRIPT_HAN) << endl; // STEP 1: A LANGUAGE HAS BEEN DEFINED, SO WE CHECK IF THE DETECTED SCRIPT ALLOWS TO WRITE IT
    std::cout << languageHelper.getDefaultLanguage(HB_SCRIPT_HAN) << endl; // STEP 2: CHECK IF THE SCRIPT CAN BE RESOLVED INTO A LANGUAGE FROM THE "DEFAULT LIST"
    
//    Test::run(" ॆहिन्दी العربية Русский English 漢孵とひらがなとカタカナ𐐀𐐁𐐂𐐃");
//    Test::run("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.");
//    Test::run("The title says \"W3C פעילות הבינאום,\u200f\" in Hebrew.");
//    Test::run("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.");
//    Test::run("ユニコードは、すべての文字に固有の番号を付与します");
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
