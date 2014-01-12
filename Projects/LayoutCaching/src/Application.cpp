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

/*
 * TODO:
 *
 * 1) ADJUST FONTS:
 *    - iOS:
 *      - "Geeza Pro" SHOULD BE REPLACED (ARABIC GLYPHS ARE NOT JOINED)
 *    - ANDROID:
 *      - HEBREW FONT IS MISSING
 *      - ARABIC FONT IS PROBABLY OUTDATED
 *      - THAI FONT IS BUGGY...
 *
 * 2) ADD SCALE-FACTOR FOR ACTUAL-FONTS IN XML DEFINITION:
 *    - NECESSARY IN CASE WE NEED TO MATCH SIZES BETWEEN "Geeza Pro" (SMALLER?) AND "Arial" (BIGGER?)
 *
 * 3) ADAPT TEXT-LAYOUT-CACHE SYSTEM TO TextGroup
 */

#include "cinder/app/AppNative.h"
#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include "LineItemizer.h"
#include "FontManager.h"

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 27;
const float LINE_TOP = 66;
const float LINE_SPACING = 66;

class Application : public AppNative
{
    FontManager fontManager;
    LineItemizer itemizer;
    VirtualFont *font;
    vector<unique_ptr<LineLayout>> lineLayouts;
    
public:
    void prepareSettings(Settings *settings);
    void setup();
    
    void draw();
    void drawLineLayout(LineLayout &layout, float y, float left, float right);
    void drawHLine(float y);
    
    void addLineLayout(const string &text, const string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_LTR);
    string trimText(const string &text) const;
    
#if defined(CINDER_ANDROID)
    void resume(bool renewContext);
#elif !defined(CINDER_COCOA_TOUCH)
    void keyDown(KeyEvent event);
#endif
    
#if defined(CINDER_ANDROID)
    inline Vec2i toPixels(Vec2i s) { return s; }
    inline float toPixels(float s) { return s; }
#endif
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1024, 736);
    settings->enableHighDensityDisplay();
    settings->disableFrameRate();
}

void Application::setup()
{
#if defined(CINDER_ANDROID)
    auto uri = "res://SansSerif-android.xml";
#elif defined(CINDER_COCOA_TOUCH)
    auto uri = "res://SansSerif-ios.xml";
#elif defined(CINDER_MAC) && 1
    auto uri = "res://SansSerif-osx.xml";
#else
    auto uri = "res://SansSerif.xml"; // FOR QUICK TESTS ON THE DESKTOP
#endif
    
    font = fontManager.getVirtualFont(uri, FONT_SIZE);
    
    // ---

    /*
     * TEXT EXAMPLES FROM THE ScriptDetector PROJECT
     */

    XmlTree doc(loadResource("Text.xml"));
    auto rootElement = doc.getChild("Text");
    
    for (auto &lineElement : rootElement.getChildren())
    {
        auto text = trimText(lineElement->getValue());
        auto language = lineElement->getAttributeValue<string>("lang", "");
        hb_direction_t direction = (lineElement->getAttributeValue<string>("dir", "") == "rtl") ? HB_DIRECTION_RTL : HB_DIRECTION_LTR;
        
        addLineLayout(text, language, direction);
    }

    /*
     * TEXT EXAMPLES FROM THE SimpleBIDI PROJECT
     */
    addLineLayout("The title is مفتاح معايير الويب in Arabic.");
    addLineLayout("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.");
    addLineLayout("The names of these states in Arabic are مصر,‎ البحرين and الكويت respectively.");
    addLineLayout("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", "", HB_DIRECTION_RTL);
    addLineLayout("The title says \"W3C פעילות הבינאום,\u200f\" in Hebrew.");
    addLineLayout("one two ثلاثة four خمسة");
    addLineLayout("one two ثلاثة 1234 خمسة");
    
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
    
    gl::clear(Color(1, 1, 0.95f), false);
    
    Vec2i windowSize = toPixels(getWindowSize());
    gl::setMatricesWindow(windowSize, true);
    
    // ---
    
    float y = LINE_TOP;
    float left = 24;
    float right = windowSize.x - 24;
    
    font->setSize(FONT_SIZE);
    
    for (auto &layout : lineLayouts)
    {
        drawLineLayout(*layout, y, left, right);
        y += LINE_SPACING;
    }
}

void Application::drawLineLayout(LineLayout &layout, float y, float left, float right)
{
    float x = (layout.overallDirection == HB_DIRECTION_LTR) ? left : (right - font->getAdvance(layout));
    Vec2f position(x, y);
    
    font->begin();
    
    for (auto cluster : layout.clusters)
    {
        font->drawCluster(cluster, position);
        position.x += font->getAdvance(cluster);
    }
    
    font->end();
    
    // ---
    
    glColor4f(0.5f, 0, 0, 0.075f);
    drawHLine(y);
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

void Application::addLineLayout(const string &text, const string &langHint, hb_direction_t overallDirection)
{
    lineLayouts.emplace_back(unique_ptr<LineLayout>(font->createLineLayout(itemizer.process(text, langHint, overallDirection))));
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

#if defined(CINDER_ANDROID)
void Application::resume(bool renewContext)
{
    if (renewContext)
    {
        fontManager.unloadTextures();
    }
}
#elif !defined(CINDER_COCOA_TOUCH)
void Application::keyDown(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_RETURN)
    {
        fontManager.unloadTextures();
    }
}
#endif

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
