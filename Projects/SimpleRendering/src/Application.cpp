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
 * 1) FONT METRICS
 *
 * 2) TEXTURE-PADDING:
 *    NECESSARY WHEN DRAWING A SMALLER SIZES WITH MIPMAPING
 *
 * 3) BASIC FONT-DRAWING AT ARBITRARY SIZE:
 *    NOT USING gl::Texture::draw ANYMORE
 *
 * 4) TextLayout CREATED VIA VirtualFont
 *
 * 5) HANDLING OPEN-GL CONTEXT-LOSS (NECESSARY FOR ANDROID):
 *    - PRESS "ENTER" TO UNLOAD ALL THE TEXTURES
 *    - GLYPH RASTERIZATION WILL THEN HAPPEN ON THE FLY...
 *
 * 6) TEXT-SIZE VARIATION
 *
 * 7) ADAPTING TO iOS AND ANDROID:
 *    - WORKS AS INTENDED ON NEXUS-7
 *    - CRASH WHEN RETURNING TO APP ON XOOM 1 (ANDROID 3):
 *      - LOOKS LIKE A GENERIC BUG IN SAFETY-DANK'S VERSION:
 *        - SHOULD NOT BE AN ISSUE WITH NEW-CHRONOTEXT-TOOLKIT
 */

/*
 * TODO:
 *
 * 1) GLYPH RENDERING:
 *    - BATCHING:
 *      - INCLUDING COLOR
 *      - MAYBE ALSO: INTERLEAVED VERTICES AND COORDS...
 *    - TRANSFORMING VERTICES VIA FontMatrix
 *    - "BEGIN / END" MODES:
 *      - "DIRECT"
 *      - "TEXTURE BUCKET"
 *      - "SEQUENCE"...
 *
 * 2) TextLayoutCache:
 *    - LRU STRATEGY?
 *
 * 3) FontManager:
 *    - PROPER ERROR HANDLING UPON CREATION
 *    - POSSIBILITY TO REMOVE A PARTICULAR VirtualFont
 *    - WE NEED A KIND OF "GLOBAL TEXTURE STORE" FOR STANDALONE-TEXTURES AND ATLASES
 */

#include "cinder/app/AppNative.h"
#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include "TextLayoutCache.h"
#include "FontManager.h"
#include "LanguageHelper.h"
#include "Test.h"

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 48;
const float LINE_TOP = 66;
const float LINE_SPACING = 66;

class Application : public AppNative
{
    LanguageHelper languageHelper;
    FontManager fontManager;
    
    VirtualFont *font;
    vector<TextRun> runs;
    TextLayoutCache layoutCache;
    
public:
    void prepareSettings(Settings *settings);
    void setup();
    
    void draw();
    void drawLineLayout(TextLayout &layout, float y, float left, float right);
    void drawHLine(float y);
    
    TextRun createRun(const string &text, const string &lang, hb_direction_t direction = HB_DIRECTION_INVALID) const;
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
    settings->setWindowSize(1280, 736);
    settings->enableHighDensityDisplay();
    settings->disableFrameRate();
}

void Application::setup()
{
#if defined(CINDER_ANDROID)
    auto ref = "res://SansSerif-android.xml";
#elif defined(CINDER_COCOA_TOUCH)
    auto ref = "res://SansSerif-ios.xml";
#elif defined(CINDER_MAC) && 1
    auto ref = "res://SansSerif-osx.xml";
#else
    auto ref = "res://SansSerif.xml"; // FOR QUICK TESTS ON THE DESKTOP
#endif

    font = fontManager.getVirtualFont(ref, FONT_SIZE);
    
    XmlTree doc(loadResource("Text.xml"));
    auto rootElement = doc.getChild("Text");
    
    for (auto &lineElement : rootElement.getChildren())
    {
        auto text = trimText(lineElement->getValue());
        auto lang = lineElement->getAttributeValue<string>("lang");
        
        runs.emplace_back(createRun(text, lang));
    }
    
#if 1
    Test::measureShaping(console(), font, runs);
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

    float size = 30 + 18 * math<float>::sin(getElapsedSeconds()); // OSCILLATING BETWEN 12 AND 48
    font->setSize(size);

    for (auto run : runs)
    {
        drawLineLayout(*layoutCache.get(font, run), y, left, right);
        y += LINE_SPACING;
    }
}

void Application::drawLineLayout(TextLayout &layout, float y, float left, float right)
{
    float x = (layout.direction == HB_DIRECTION_LTR) ? left : (right - font->getAdvance(layout));
    Vec2f position(x, y);
 
    glColor4f(1, 1, 1, 1);
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

TextRun Application::createRun(const string &text, const string &lang, hb_direction_t direction) const
{
    auto script = languageHelper.getScript(lang);
    
    if (direction == HB_DIRECTION_INVALID)
    {
        direction = hb_script_get_horizontal_direction(script);
    }
    
    return TextRun(text, script, lang, direction);
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
