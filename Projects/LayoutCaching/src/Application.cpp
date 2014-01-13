/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * DONE:
 *
 * 1) LayoutCache IN PLACE:
 *    - ADAPTED TO TextLine
 *    - LRU CACHE STRATEGY BASED ON boost::bimaps
 */

/*
 * TODO:
 *
 * 1) TEST LRU CACHE ON iOS AND ANDROID
 *
 * 2) ADJUST FONTS:
 *    - iOS:
 *      - "Geeza Pro" SHOULD BE REPLACED (ARABIC GLYPHS ARE NOT JOINED)
 *    - ANDROID:
 *      - HEBREW FONT IS MISSING
 *      - ARABIC FONT IS PROBABLY OUTDATED
 *      - THAI FONT IS BUGGY...
 *
 * 3) ADD SCALE-FACTOR FOR ACTUAL-FONTS IN XML DEFINITION:
 *    - NECESSARY IN CASE WE NEED TO MATCH SIZES BETWEEN "Geeza Pro" (SMALLER?) AND "Arial" (BIGGER?)
 */

#include "cinder/app/AppNative.h"
#include "cinder/Xml.h"
#include "cinder/Utilities.h"
#include "cinder/Rand.h"

#include "FontManager.h"
#include "LayoutCache.h"

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 27;
const float LINE_TOP = 66;
const float LINE_SPACING = 66;

const int LINE_COUNT = 11;
const int MAX_WORDS_PER_LINE = 4;

class Application : public AppNative
{
    FontManager fontManager;
    LayoutCache layoutCache;
    
    VirtualFont *font;
    vector<unique_ptr<LineLayout>> lineLayouts;

    vector<string> words;
    Rand rnd;

public:
    void prepareSettings(Settings *settings);
    void setup();
    
    void draw();
    void drawLineLayout(LineLayout &layout, float y, float left, float right);
    void drawHLine(float y);
    
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
#elif defined(CINDER_MAC) && 0
    auto uri = "res://SansSerif-osx.xml";
#else
    auto uri = "res://SansSerif.xml"; // FOR QUICK TESTS ON THE DESKTOP
#endif
    
    font = fontManager.getVirtualFont(uri, FONT_SIZE);
    
    // ---

    XmlTree doc(loadResource("Words.xml"));
    auto rootElement = doc.getChild("Text");
    
    for (auto &lineElement : rootElement.getChildren())
    {
        auto text = trimText(lineElement->getValue());
        words.push_back(text);
    }
    
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
    
    font->setSize(FONT_SIZE);
    font->setColor(ColorA(1, 1, 1, 0.75f));
    
    for (int i = 0; i < LINE_COUNT; i++)
    {
        string line;
        int wordCount = rnd.nextInt(1, MAX_WORDS_PER_LINE);
        
        for (int j = 0; j < wordCount; j++)
        {
            line += words[rnd.nextInt(words.size())];
            line += " ";
        }
        
        drawLineLayout(*layoutCache.getLineLayout(font, line), y, left, right);
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
    
    glColor4f(1, 0.75f, 0, 0.25f);
    drawHLine(y);
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
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
        layoutCache.clear();
    }
}
#endif

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
