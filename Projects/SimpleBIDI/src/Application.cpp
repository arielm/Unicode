/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * REFERENCE: https://github.com/arielm/Unicode/tree/master/Projects/SimpleBIDI
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"
#include "TextLayout.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 32;
const float LINE_TOP = 90;
const float LINE_SPACING = 90;

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    
    map<hb_script_t, FontList> fontMap;
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
    settings->enableHighDensityDisplay();
    settings->disableFrameRate();
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();
    
    auto font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSans.ttf")), FONT_SIZE);
    auto font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSansHebrew-Regular.ttf")), FONT_SIZE);
    auto font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidNaskh-Regular.ttf")), FONT_SIZE);
    
    fontMap[HB_SCRIPT_HEBREW].push_back(font2);
    fontMap[HB_SCRIPT_HEBREW].push_back(font1);
    
    fontMap[HB_SCRIPT_ARABIC].push_back(font3);
    fontMap[HB_SCRIPT_ARABIC].push_back(font1);
    
    // ---

    lineLayouts.emplace_back(fontMap, TextGroup("The title is مفتاح معايير الويب in Arabic.", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontMap, TextGroup("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontMap, TextGroup("The names of these states in Arabic are مصر,‎ البحرين and الكويت respectively.", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontMap, TextGroup("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", HB_SCRIPT_HEBREW, "he", HB_DIRECTION_RTL));
    lineLayouts.emplace_back(fontMap, TextGroup("The title says \"W3C פעילות הבינאום,\u200f\" in Hebrew.", HB_SCRIPT_HEBREW, "he", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontMap, TextGroup("one two ثلاثة four خمسة", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    lineLayouts.emplace_back(fontMap, TextGroup("one two ثلاثة 1234 خمسة", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR));
    
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
    float x = (layout.overallDirection == HB_DIRECTION_LTR) ? left : (right - layout.advance);
    layout.draw(Vec2f(x, y));
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
