/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * TESTING LANGUAGE-SPECIFIC FEATURES, AS DESCRIBE IN:
 * http://www.mail-archive.com/harfbuzz@lists.freedesktop.org/msg03194.html
 *
 *
 * RESULTS:
 *
 * - WEBKIT EXAMPLE: IT WORKS ONLY FOR THE FIRST LETTER
 * - PANGO EXAMPLE: IT WORKS AS INTENDED
 * - SCHEHERAZADE EXAMPLE: IT DOES NOT WORK AS INTENDED:
 *   - TRYING "snd" INSTEAD OF "sd" FOR LANGUAGE IS NOT HELPING...
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 56;
const fs::path EXTERNAL_FONTS_DIRECTORY = "/Users/arielm/Downloads/fonts";

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
    void drawSpan(const YFont &font, const TextSpan &span1, const TextSpan &span2, float y);
    void drawHLine(float y);
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1024, 512);
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();
    
    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile(EXTERNAL_FONTS_DIRECTORY / "dejavu-fonts-ttf-2.34/ttf/DejaVuSerif.ttf")), FONT_SIZE);
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/Library/Fonts/Verdana.ttf")), FONT_SIZE);
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile(EXTERNAL_FONTS_DIRECTORY / "Scheherazade-2.010/Scheherazade-R.ttf")), FONT_SIZE);
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
    gl::setMatricesWindow(toPixels(getWindowSize()), true);
    
    drawSpan(*font1, TextSpan("бгдпт", HB_SCRIPT_CYRILLIC, HB_DIRECTION_LTR, "ru"), TextSpan("бгдпт", HB_SCRIPT_CYRILLIC, HB_DIRECTION_LTR, "sr"), 128);
    drawSpan(*font2, TextSpan("şţ", HB_SCRIPT_LATIN, HB_DIRECTION_LTR, "en"), TextSpan("şţ", HB_SCRIPT_LATIN, HB_DIRECTION_LTR, "ro"), 256);
    drawSpan(*font3, TextSpan("ههه ۴۵۶۷", HB_SCRIPT_ARABIC, HB_DIRECTION_RTL, "ar"), TextSpan("ههه ۴۵۶۷", HB_SCRIPT_ARABIC, HB_DIRECTION_RTL, "sd"), 384);
}

void Application::drawSpan(const YFont &font, const TextSpan &span1, const TextSpan &span2, float y)
{
    glColor4f(1, 1, 1, 1);
    font.drawSpan(span1, 24, y);
    font.drawSpan(span2, 512 + 24, y);
    
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
