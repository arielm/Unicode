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
 * TRY TO PERFORM ITEMIZATION (SCRIPT + BIDI), LIKE IN:
 *
 * 1) SIMPLE: https://github.com/herm/harfbuzz-test/blob/master/text_itemizer.cpp
 *
 * 2) BETTER: https://github.com/mapnik/mapnik/blob/master/src/text/itemizer.cpp
 *
 * 3) IDEAL(?): https://github.com/android/platform_frameworks_base/blob/677726b376402937f53ddb192dc97078b92b7c9e/core/jni/android/graphics/TextLayoutCache.cpp
 *
 * NOTE: THESE 3 EXAMPLES ARE ALSO HANDLING ITEM-STYLE...
 */

#include "cinder/app/AppNative.h"

#include "TextItem.h"

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
    TextItem item("לְהַגִּיד בַּבֹּקֶר חַסְדֶּךָ וֶאֱמוּנָתְךָ בַּלֵּילוֹת", USCRIPT_HEBREW, UBIDI_RTL);
    cout << item.getUTF8Text() << endl;
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
