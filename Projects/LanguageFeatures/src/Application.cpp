/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * TESTING LANGUAGE-SPECIFIC FEATURES, AS A FOLLOW-UP TO:
 * http://www.mail-archive.com/harfbuzz@lists.freedesktop.org/msg03194.html
 *
 *
 * RESULTS:
 *
 * 1) RUSSIAN VS CYRILLIC:
 *    - IT WORKS ONLY FOR THE FIRST LETTER
 *    - REFERENCE: https://bugs.webkit.org/show_bug.cgi?id=37984
 *
 * - ENGLISH VS ROMANIAN:
 *   - IT WORKS AS INTENDED
 *   - REFERENCE: http://en.wikipedia.org/wiki/Pango
 *
 * - ARABIC VS SINDHI:
 *   - IT WORKS AS INTENDED WITH ScheherazadeRegOT.ttf
 *   - REFERENCE: http://scripts.sil.org/cms/scripts/page.php?item_id=Scheherazade
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 56;

struct MultiSpan
{
    std::string text;
    hb_script_t script;
    hb_direction_t direction;
    
    std::string lang1;
    std::string lang2;
    
    MultiSpan(const std::string &text, hb_script_t script, hb_direction_t direction, const std::string &lang1, const std::string &lang2)
    :
    text(text),
    script(script),
    direction(direction),
    lang1(lang1),
    lang2(lang2)
    {}
};

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
    void drawSpans(const YFont &font, const MultiSpan &spans, float y);
    void drawHLine(float y);
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1024, 512);
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();

    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadResource("DejaVuSerif.ttf")), FONT_SIZE);
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadFile("/Library/Fonts/Verdana.ttf")), FONT_SIZE); // OSX
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadResource("ScheherazadeRegOT.ttf")), FONT_SIZE);
    
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
    
    drawSpans(*font1, MultiSpan("бгдпт", HB_SCRIPT_CYRILLIC, HB_DIRECTION_LTR, "ru", "sr"), 128);
    drawSpans(*font2, MultiSpan("şţ", HB_SCRIPT_LATIN, HB_DIRECTION_LTR, "en", "ro"), 256);
    drawSpans(*font3, MultiSpan("ههه ۴۵۶۷", HB_SCRIPT_ARABIC, HB_DIRECTION_RTL, "ar", "sd"), 384);
}

void Application::drawSpans(const YFont &font, const MultiSpan &spans, float y)
{
    glColor4f(1, 1, 1, 1);
    font.drawSpan(TextSpan(spans.text, spans.script, spans.direction, spans.lang1), 24, y);
    font.drawSpan(TextSpan(spans.text, spans.script, spans.direction, spans.lang2), 512 + 24, y);
    
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
