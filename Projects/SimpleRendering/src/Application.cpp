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
 *    NOT USING gl::Texture ANYMORE
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
 *    - CRASH WHEN RETURNING TO APP ON XOOM 1 (ANDROID 3)
 *
 * 8) GLYPH-RENDERING NOW WORKS WITH glColorPointer
 */

/*
 * TODO:
 *
 * 1) ANDROID CRASH WHEN RETURNING TO APP ON XOOM 1:
 *    SEE IF IT OCCURS WHEN USING new-chronotext-toolkit
 *    (I.E. INSTEAD OF USING A SOLUTION BASED ON NativeActivity)
 *
 * 2) ADVANCED GLYPH RENDERING:
 *    - BATCHING ("TEXTURE BUCKET"):
 *      - USING A MAP, WITH AN ENTRY FOR EACH TEXTURE:
 *        - EACH ENTRY CONTAINING A VECTOR OF FLOATS FILLED-WITH:
 *          - INTERLEAVED POSITIONS, TEXTURE-COORDS AND COLORS
 *    - TRANSFORMING VERTICES VIA FontMatrix
 *    - "BEGIN / END" MODES:
 *      - "DIRECT"
 *      - "TEXTURE BUCKET"
 *
 * 3) TEXTURE-ATLASES:
 *    - SIMILAR TO THE SYSTEM USED IN XFont, BUT WITH N TEXTURES
 *    - WE NEED A SYSTEM FOR USERS TO DEFINE WHICH GLYPHS ARE CACHED
 *      - BECAUSE OF MIPMAPPING, WE CAN'T ADD GLYPHS ON-THE-FLY
 *      - THERE SHOULD BE A WAY TO ADD/REMOVE GROUPS OF GLYPHS, E.G. PER LANGUAGE
 *
 * 4) TextLayoutCache:
 *    - LRU STRATEGY
 *
 * 5) FontManager:
 *    - PROPER ERROR HANDLING UPON CREATION
 *    - FONT QUERYING:
 *      - HANDLING WEIGHTS (NORMAL BOLD)
 *      - HANDLING OPERATING SYSTEMS (ANDROID, iOS, ETC.)
 *    - POSSIBILITY TO REMOVE A PARTICULAR VirtualFont
 *    - WE NEED A KIND OF "GLOBAL TEXTURE STORE" FOR STANDALONE-TEXTURES AND ATLASES:
 *      - LRU STRATEGY FOR STANDALONE-TEXTURES
 *      - AUTOMATIC REMOVAL OF STANDALONE-TEXTURE WHENEVER AN ATLAS IS STORING THE ASSOCIATED GLYPH'S TEXTURE
 */

#include "cinder/app/AppNative.h"
#include "cinder/Xml.h"
#include "cinder/Utilities.h"

#include "TextLayoutCache.h"
#include "FontManager.h"
#include "LanguageHelper.h"

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
    auto uri = "res://SansSerif-android.xml";
#elif defined(CINDER_COCOA_TOUCH)
    auto uri = "res://SansSerif-ios.xml";
#elif defined(CINDER_MAC) && 1
    auto uri = "res://SansSerif-osx.xml";
#else
    auto uri = "res://SansSerif.xml"; // FOR QUICK TESTS ON THE DESKTOP
#endif

    font = fontManager.getVirtualFont(uri, FONT_SIZE);
    
    XmlTree doc(loadResource("Text.xml"));
    auto rootElement = doc.getChild("Text");
    
    for (auto &lineElement : rootElement.getChildren())
    {
        auto text = trimText(lineElement->getValue());
        auto lang = lineElement->getAttributeValue<string>("lang");
        
        runs.emplace_back(createRun(text, lang));
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

    float size = 30 + 18 * math<float>::sin(getElapsedSeconds()); // OSCILLATING BETWEEN 12 AND 48
    font->setSize(size);
    
    font->setColor(ColorA(1, 1, 1, 0.75f));

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
