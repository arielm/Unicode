/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * REFERENCE: https://github.com/arielm/Unicode/tree/master/Projects/SimpleVirtualFont
 */

/*
 * TODO:
 *
 * 0) FURTHER INVESTIGATE iOS "APPSTORE REJECTION RISK":
 *    - http://stackoverflow.com/questions/3692812/on-ios-can-i-access-the-system-provided-fonts-ttf-file
 *    - https://github.com/WebKit/webkit/blob/master/Source/WebCore/platform/graphics/harfbuzz/HarfBuzzFaceCoreText.cpp
 */

#include "cinder/app/AppNative.h"
#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include "TextLayout.h"
#include "FontManager.h"
#include "LanguageHelper.h"
#include "Test.h"

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 32;
const float LINE_TOP = 66;
const float LINE_SPACING = 66;

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
    
    TextSpan createRun(const string &text, const string &lang) const;
    string trimText(const string &text) const;
    
#if defined(CINDER_ANDROID)
    void resume(bool renewContext) {}
    inline Vec2i toPixels(Vec2i s) { return s; }
    inline float toPixels(float s) { return s; }
#endif
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1280, 736);
    settings->enableHighDensityDisplay();
    settings->disableFrameRate();
}

void Application::setup()
{
#if defined(CINDER_ANDROID)
    auto fileName = "SansSerif-android.xml";
#elif defined(CINDER_COCOA_TOUCH)
    auto fileName = "SansSerif-ios.xml";
#elif defined(CINDER_MAC) && 1
    auto fileName = "SansSerif-osx.xml";
#else
    auto fileName = "SansSerif.xml"; // FOR QUICK TESTS ON THE DESKTOP
#endif

    auto virtualFont = fontManager.loadVirtualFont(loadResource(fileName), FONT_SIZE);
    vector<TextSpan> runs;
    
    XmlTree doc(loadResource("Text.xml"));
    auto rootElement = doc.getChild("Text");
    
    for (auto &lineElement : rootElement.getChildren())
    {
        auto lang = lineElement->getAttributeValue<string>("lang");
        auto text = trimText(lineElement->getValue());
        runs.emplace_back(createRun(text, lang));
    }
    
    for (auto run : runs)
    {
        lineLayouts.emplace_back(virtualFont, run);
    }

#if 0
    Test::measureShaping(console(), virtualFont, runs);
#endif
    
    // ---
    
#if defined(CINDER_COCOA_TOUCH)
    getSignalSupportedOrientations().connect([] { return InterfaceOrientation::LandscapeRight; });
#endif
}

void Application::draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    // ---
    
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

string Application::trimText(const string &text) const
{
    auto rawLines = split(text, '\n');
    
    for (auto line : rawLines)
    {
        auto trimmed = boost::algorithm::trim_copy(line);
        
        if (!trimmed.empty())
        {
            return trimmed;
        }
    }
    
    return "";
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
