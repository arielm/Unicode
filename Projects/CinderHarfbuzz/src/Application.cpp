/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * REQUIREMENTS:
 * - THE REPOSITORY SHOULD BE PLACED UNDER $CINDER/samples
 * - DEPENDS ON THIS BLOCK (TO BE PLACED UNDER $CINDER/blocks): https://github.com/arielm/Freetype
 *
 * OSX AND iOS:
 * - COMPILED ON OSX 10.5.8 WITH XCode 5.0.2 AND CINDER "VANILLA" 0.8.5
 *
 * ANDROID:
 * - COMPILED ON OSX 10.5.8 WITH:
 *   - NDK R8E: http://dl.google.com/android/ndk/android-ndk-r8e-darwin-x86_64.tar.bz2
 *   - JULY 29TH VERSION OF ANDROID SDK: http://dl.google.com/android/adt/adt-bundle-mac-x86_64-20130729.zip
 *   - SAFETYDANK'S FORK OF CINDER: https://github.com/safetydank/Cinder
 *
 * WINDOWS:
 * - SHOULD BE TRIVIALLY ADAPTABLE TO VISUAL STUDIO 2012
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"

using namespace std;
using namespace ci;
using namespace app;

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
    void drawLine(float y);
    
#if defined(CINDER_ANDROID)
    inline Vec2i toPixels(Vec2i s) { return s; }
    inline float toPixels(float s) { return s; }
#endif
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1024, 512);
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();
    
#if defined(CINDER_ANDROID) && 0 // USE "1" FOR CUSTOM FONTS ON ANDROID (TESTED ON NEXUS-7)
    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/system/fonts/DroidSans.ttf")), 56); // SYM-LINKED TO Roboto-Regular.ttf ON ANDROID 4+
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/system/fonts/DroidSansHebrew-Regular.ttf")), 56);
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/system/fonts/DroidNaskh-Regular.ttf")), 56);
#elif defined(CINDER_MAC) && 0 // USE "1" FOR CUSTOM FONTS ON OSX (TESTED ON 10.8.5)
    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/System/Library/Fonts/Helvetica.dfont"), 4), 56); // FACE-INDEX 4 CORRESPONDS TO "Helvetica Regular"
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/Library/Fonts/AdobeHebrew-Regular.otf")), 56);
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/Library/Fonts/AdobeArabic-Regular.otf")), 56);
#else // CUSTOM FONTS LOADED FROM THE RESOURCE-BUNDLE ON OSX AND iOS OR FROM THE ASSETS ON ANDROID
    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadResource("DroidSans.ttf")), 56);
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadResource("DroidSansHebrew-Regular.ttf")), 56);
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadResource("DroidSansArabic.ttf")), 56);
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
    
    drawSpan(*font1, TextSpan("Exquis, exquis! Vive la mère Ubu.", HB_SCRIPT_LATIN, HB_DIRECTION_LTR, "fr") , 100); // http://goo.gl/TG3BPN
    drawSpan(*font2, TextSpan("לְהַגִּיד בַּבֹּקֶר חַסְדֶּךָ וֶאֱמוּנָתְךָ בַּלֵּילוֹת", HB_SCRIPT_HEBREW, HB_DIRECTION_RTL, "he") , 250); // http://goo.gl/9UBdl8
    drawSpan(*font3, TextSpan("ما هي الشفرة الموحدة يونيكود؟", HB_SCRIPT_ARABIC, HB_DIRECTION_RTL, "ar") , 400); // http://goo.gl/1xErNU
}

void Application::drawSpan(const YFont &font, const TextSpan &span, float y)
{
    glColor4f(1, 1, 1, 1);
    font.drawSpan(span, 32, y);
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawLine(y);
    
    glColor4f(1, 1, 0, 0.33f);
    drawLine(y - font.ascent);
    drawLine(y + font.descent);
}

void Application::drawLine(float y)
{
    gl::drawLine(Vec2f(0, y), Vec2f(toPixels(getWindowWidth()), y));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
