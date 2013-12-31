/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "cinder/app/AppNative.h"
#include "cinder/Utilities.h"

#include "YFont.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 56;

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    
    shared_ptr<YFont> font1;
    shared_ptr<YFont> font2;
    shared_ptr<YFont> font3;
   
public:
    void prepareSettings(Settings *settings);
    void setup();
    
    void draw();
    void drawSpan(const YFont &font, const TextSpan &span, float y);
    void drawHLine(float y);
    
#if defined(CINDER_ANDROID)
    inline Vec2i toPixels(Vec2i s) { return s; }
    inline float toPixels(float s) { return s; }
#endif
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1024, 512);
    settings->enableHighDensityDisplay();
    settings->disableFrameRate();
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();
    
#if defined(CINDER_ANDROID) && 0 // USE "1" FOR SYSTEM FONTS ON ANDROID (TESTED ON NEXUS-7)
    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/system/fonts/DroidSans.ttf")), FONT_SIZE); // SYM-LINKED TO Roboto-Regular.ttf ON ANDROID 4+
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/system/fonts/DroidSansHebrew-Regular.ttf")), FONT_SIZE);
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/system/fonts/DroidNaskh-Regular.ttf")), FONT_SIZE);
#elif defined(CINDER_MAC) && 0 // USE "1" FOR SYSTEM FONTS ON OSX (TESTED ON 10.8.5)
    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/System/Library/Fonts/Helvetica.dfont"), 4), FONT_SIZE); // FACE-INDEX 4 CORRESPONDS TO "Helvetica Regular"
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/Library/Fonts/AdobeHebrew-Regular.otf")), FONT_SIZE);
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/Library/Fonts/AdobeArabic-Regular.otf")), FONT_SIZE);
#elif defined(CINDER_COCOA_TOUCH) && 1 // USE "1" FOR SYSTEM FONTS ON iOS (TESTED ON 5.0.1)
    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/System/Library/Fonts/Cache/Arial.ttf")), FONT_SIZE);
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/System/Library/Fonts/Cache/ArialHB.ttf")), FONT_SIZE);
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/System/Library/Fonts/Cache/GeezaPro.ttf")), FONT_SIZE); // FIXME: LOOKS LIKE "JOINING" IS BROKEN WITH THAT FONT (NOT ON OSX)
#else // CUSTOM FONTS LOADED FROM THE RESOURCE-BUNDLE ON OSX AND iOS OR FROM THE ASSETS ON ANDROID
    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadResource("DroidSans.ttf")), FONT_SIZE);
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadResource("DroidSansHebrew-Regular.ttf")), FONT_SIZE);
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadResource("DroidSansArabic.ttf")), FONT_SIZE);
#endif

    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
#if defined(CINDER_COCOA_TOUCH)
    getSignalSupportedOrientations().connect([] { return InterfaceOrientation::LandscapeRight; });
#endif
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
    gl::setMatricesWindow(toPixels(getWindowSize()), true);
    
    drawSpan(*font1, TextSpan("Exquis, exquis! Vive la mère Ubu.", HB_SCRIPT_LATIN, HB_DIRECTION_LTR, "fr") , 128); // http://goo.gl/TG3BPN
    drawSpan(*font2, TextSpan("לְהַגִּיד בַּבֹּקֶר חַסְדֶּךָ וֶאֱמוּנָתְךָ בַּלֵּילוֹת", HB_SCRIPT_HEBREW, HB_DIRECTION_RTL, "he") , 256); // http://goo.gl/9UBdl8
    drawSpan(*font3, TextSpan("ما هي الشفرة الموحدة يونيكود؟", HB_SCRIPT_ARABIC, HB_DIRECTION_RTL, "ar") , 384); // http://goo.gl/1xErNU
}

void Application::drawSpan(const YFont &font, const TextSpan &span, float y)
{
    glColor4f(1, 1, 1, 1);
    font.drawSpan(span, 32, y);
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
    
    glColor4f(1, 1, 0, 0.33f);
    drawHLine(y - font.ascent);
    drawHLine(y + font.descent);
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
