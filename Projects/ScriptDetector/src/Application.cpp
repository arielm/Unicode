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
 * 1) SHAPE SCRIPT/LANGUAGE ITEMS BASED ON ICU-STRING CHUNKS (I.E. DEFINED VIA START/END INDICES...)
 *
 * 2) NEXT PROJECT:
 *    BIDI ITEMIZATION SHOULD TAKE PLACE,
 *    THEN THE SCRIPT/LANGUAGE AND BIDI ITEMS SHOULD BE "MIXED",
 *    AS DESCRIBED IN http://www.mail-archive.com/harfbuzz@lists.freedesktop.org/msg03190.html
 *    AND AS IMPLEMENTED IN THE MAPNIK PROJECT
 */

#include "cinder/app/AppNative.h"

#include "TextItemizer.h"

using namespace std;
using namespace ci;
using namespace app;

class Application : public AppNative
{
    TextItemizer itemizer;
    
public:
    void setup();
    void draw();
};

void Application::setup()
{
//  Test().run();
    
    auto group1 = itemizer.process("ユニコードは、すべての文字に固有の番号を付与します", "ja");
    auto group2 = itemizer.process(" ॆहिन्दी العربية Русский English 漢孵とひらがなとカタカナ𐐀𐐁𐐂𐐃");
    auto group3 = itemizer.process("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.");
    auto group4 = itemizer.process("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", "", HB_DIRECTION_RTL);
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
