/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * ...
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

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    
    FontTree fontTree;
    vector<TextLayout> lineLayouts;
    
public:
    void prepareSettings(Settings *settings);
    void setup();
    
    void draw();
    void drawLineLayout(TextLayout &layout, float y, float left, float right);
    void drawHLine(float y);
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1280, 720);
}

void Application::setup()
{
//    size_t count = sizeof(HB_SCRIPT_for_lang) / sizeof(HBScriptForLang);
//
//    for (int i = 0; i < count; i++)
//    {
//        cout << HB_SCRIPT_for_lang[i].lang << " | " << HB_SCRIPT_for_lang[i].scripts[0] << endl;
//    }
//    
//    exit(0);
    
    // ---
    
    ftHelper = make_shared<FreetypeHelper>();
    
    auto font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSans.ttf")), FONT_SIZE);
    auto font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSansHebrew-Regular.ttf")), FONT_SIZE);
    auto font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidNaskh-Regular.ttf")), FONT_SIZE);
    
    fontTree[HB_SCRIPT_HEBREW].push_back(font2);
    fontTree[HB_SCRIPT_HEBREW].push_back(font1);
    
    fontTree[HB_SCRIPT_ARABIC].push_back(font3);
    fontTree[HB_SCRIPT_ARABIC].push_back(font1);
    
    // ---

    lineLayouts.emplace_back(fontTree, TextSpan("The title is مفتاح معايير الويب in Arabic.", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontTree, TextSpan("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontTree, TextSpan("The names of these states in Arabic are مصر,‎ البحرين and الكويت respectively.", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontTree, TextSpan("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", HB_SCRIPT_HEBREW, "he", HB_DIRECTION_RTL));
    lineLayouts.emplace_back(fontTree, TextSpan("The title says \"W3C פעילות הבינאום,\u200f\" in Hebrew.", HB_SCRIPT_HEBREW, "he", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontTree, TextSpan("one two ثلاثة four خمسة", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontTree, TextSpan("one two ثلاثة 1234 خمسة", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    
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

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
