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
 * 1) USE FontTree.xml INSTEAD OF CREATING sansSerifFont BY CODE
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"
#include "TextLayout.h"

#include "Language.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 32;
const float LINE_TOP = 90;
const float LINE_SPACING = 90;

typedef std::map<std::string, FontList> FontTree;

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    map<string, hb_script_t> scriptMap;
    
    FontTree sansSerifFont;
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
    size_t count = sizeof(HB_SCRIPT_for_lang) / sizeof(HBScriptForLang);

    for (int i = 0; i < count; i++)
    {
        scriptMap[HB_SCRIPT_for_lang[i].lang] = HB_SCRIPT_for_lang[i].scripts[0];
    }

    // ---
    
    ftHelper = make_shared<FreetypeHelper>();
    
    auto font0 = make_shared<YFont>(ftHelper, getAssetPath("fonts/DroidSansFallback.ttf"), FONT_SIZE);
    auto font1 = make_shared<YFont>(ftHelper, getAssetPath("fonts/DroidSans.ttf"), FONT_SIZE);
    auto font2 = make_shared<YFont>(ftHelper, getAssetPath("fonts/DroidSansHebrew-Regular.ttf"), FONT_SIZE);
    auto font3 = make_shared<YFont>(ftHelper, getAssetPath("fonts/DroidNaskh-Regular.ttf"), FONT_SIZE);
    auto font4 = make_shared<YFont>(ftHelper, getAssetPath("fonts/MTLmr3m.ttf"), FONT_SIZE);
    auto font5 = make_shared<YFont>(ftHelper, "/Users/arielm/Downloads/fonts/NanumGothic/NanumGothic-Regular.ttf", FONT_SIZE);
    auto font6 = make_shared<YFont>(ftHelper, getAssetPath("fonts/DroidSansThai.ttf"), FONT_SIZE);

    sansSerifFont[""].push_back(font0);
    
    sansSerifFont["ar"].push_back(font3);
    sansSerifFont["ar"].push_back(font0);

    sansSerifFont["el"].push_back(font1);
    sansSerifFont["el"].push_back(font0);

    sansSerifFont["he"].push_back(font2);
    sansSerifFont["he"].push_back(font0);
    
    sansSerifFont["ja"].push_back(font4);
    sansSerifFont["ja"].push_back(font0);
    
    sansSerifFont["ko"].push_back(font5);
    sansSerifFont["ko"].push_back(font0);
    
    sansSerifFont["th"].push_back(font6);
    sansSerifFont["th"].push_back(font0);

    // ---

    lineLayouts.emplace_back(createLayout(sansSerifFont, "Press 'play' when ready!", "en"));
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
    hb_script_t script;
    auto it = scriptMap.find(lang);
    
    if (it == scriptMap.end())
    {
        script = HB_SCRIPT_INVALID;
    }
    else
    {
        script = it->second;
    }
    
    return TextSpan(text, script, lang, hb_script_get_horizontal_direction(script));
}

TextLayout Application::createLayout(FontTree &fontTree, const string &text, const string &lang) const
{
    auto it = fontTree.find(lang);
    
    if (it == fontTree.end())
    {
        return TextLayout(fontTree[""], createRun(text, lang));
    }
    else
    {
        return TextLayout(it->second, createRun(text, lang));
    }
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
