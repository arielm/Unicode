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
 *    IS PROVIDED FOR THE LANGUAGE IN QUESTION (VIA THE XML VIRTUAL-FONT SYSTEM)
 *
 * 2) TEXT IS FROM pango-language-sample-table.h
 *
 *
 * TODO:
 *
 * 1) PROVIDE "METRICS" PER FONT-SET:
 *    - ASCENT, DESCENT, HEIGHT AND STRIKETHROUGH OFFSET:
 *      - BASED ON FIRST FONT IN SET
 *
 * 2) READ TEXT FROM XML FILE
 *
 * 3) CREATE VIRTUAL-FONT FOR OSX AND iOS
 *    AND CREATE iOS AND ANDROID PROJECTS
 *
 * 4) START TO MEASURE PERFORMANCE ON iOS AND ANDROID:
 *    - SHAPING
 *    - RENDERING
 */

#include "cinder/app/AppNative.h"

#include "TextLayout.h"
#include "FontManager.h"
#include "LanguageHelper.h"

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
    
    TextLayout createLayout(VirtualFont &font, const string &text, const string &lang) const;
    TextSpan createRun(const string &text, const string &lang) const;
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1280, 736);
}

void Application::setup()
{
    auto sansSerifFont = fontManager.loadVirtualFont(loadResource("SansSerif.xml"), FONT_SIZE);

    lineLayouts.emplace_back(createLayout(sansSerifFont, "Чуєш їх, доцю, га? Кумедна ж ти, прощайся без ґольфів!", "uk"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "Θέλει αρετή και τόλμη η ελευθερία. (Ανδρέας Κάλβος)", "el"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "نص حكيم له سر قاطع وذو شأن عظيم مكتوب على ثوب أخضر ومغلف بجلد أزرق.", "ar"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "איך קען עסן גלאָז און עס טוט מיר נישט װײ.", "yi"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "いろはにほへと ちりぬるを 色は匂へど 散りぬるを", "ja"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "다람쥐 헌 쳇바퀴에 타고파", "ko"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "เป็นมนุษย์สุดประเสริฐเลิศคุณค่า - กว่าบรรดาฝูงสัตว์เดรัจฉาน ...", "th"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "नहीं नजर किसी की बुरी नहीं किसी का मुँह काला जो करे सो उपर वाला", "hi"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "我能吞下玻璃而不伤身体。", "zh-cn"));
    lineLayouts.emplace_back(createLayout(sansSerifFont, "Příliš žluťoučký kůň úpěl ďábelské ódy.", "cz"));
    
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

TextLayout Application::createLayout(VirtualFont &font, const string &text, const string &lang) const
{
    return TextLayout(font.getFontSet(lang), createRun(text, lang));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
