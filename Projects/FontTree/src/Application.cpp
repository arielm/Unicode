/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * FEATURES:
 *
 * 1) DISPLAYING TEXT IN ANY LANGUAGE, AS LONG AS THE RIGHT LIST OF FONTS (INCL. FALLBACK)
 *    IS PROVIDED FOR THE LANGUAGE IN QUESTION (VIA THE XML FONT-TREE SYSTEM)
 *
 *
 * TODO:
 *
 * 1) PROVIDE "METRICS" PER FONT-LIST:
 *    - ASCENT, DESCENT: BASED ON FIRST FONT IN LIST
 *    - STRIKETHROUGH: BASED ON FIRST FONT IN LIST WITH "-" CHARACTER, OTHERWISE: EQUAL TO 0.5
 */

#include "cinder/app/AppNative.h"

#include "TextLayout.h"
#include "FontManager.h"
#include "LanguageHelper.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 32;
const float LINE_TOP = 90;
const float LINE_SPACING = 90;

class Application : public AppNative
{
    LanguageHelper languageHelper;
    FontManager fontManager;
    
    vector<TextLayout> lineLayouts;
    
public:
    void prepareSettings(Settings *settings);
    void setup();
    
    void draw();
    void drawLineLayout(TextLayout &layout, float y, float left, float right);
    void drawHLine(float y);
    
    TextLayout createLayout(FontTree &fontTree, const string &text, const string &lang) const;
    TextSpan createRun(const string &text, const string &lang) const;
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1280, 720);
}

void Application::setup()
{
    auto sansSerifFont = fontManager.loadFontTree(loadResource("SansSerif.xml"), FONT_SIZE);

    lineLayouts.emplace_back(createLayout(sansSerifFont, "Натисни «Грати», коли будеш готовий!", "uk"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "ضغطوا على \"تشغيل\" عند الاستعداد!", "ar"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "Πάτα \u2018παιχνίδι\u2019 όταν είσαι έτοιμος!", "el"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "לַחֲצוּ עַל הַּכַפְּתוֹר כְּשֶּתִהְיוּ מוּכָנִים לִמְנוֹת סוּרִיקָטוֹת!", "he"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "じゅんびが できたら [プレイ] を おしてね!", "ja"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "준비가 되었으면 '재생'을 누르세요!", "ko"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "ถ้าพร้อมแล้ว กด 'เล่น' กันเลย!", "th"));
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
    
    Vec2i windowSize = toPixels(getWindowSize());
    gl::setMatricesWindow(windowSize, true);

    // ---

    float y = LINE_TOP;
    float left = 24;
    float right = windowSize.x - 24;
    
    for (auto layout : lineLayouts)
    {
        drawLineLayout(layout, y, left, right);
        y += LINE_SPACING;
    }
}

void Application::drawLineLayout(TextLayout &layout, float y, float left, float right)
{
    float x = (layout.direction == HB_DIRECTION_LTR) ? left : (right - layout.advance);
    
    glColor4f(1, 1, 1, 1);
    layout.draw(Vec2f(x, y));
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

TextSpan Application::createRun(const string &text, const string &lang) const
{
    auto script = languageHelper.getScript(lang);
    auto direction = hb_script_get_horizontal_direction(script);
    
    return TextSpan(text, script, lang, direction);
}

TextLayout Application::createLayout(FontTree &fontTree, const string &text, const string &lang) const
{
    return TextLayout(fontTree.getFontList(lang), createRun(text, lang));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
