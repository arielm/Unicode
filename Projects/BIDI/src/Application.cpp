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
 * 1) RENDER RUNS WITH HARFBUZZ
 *    - INSIGHT: HARFBUZZ-DIRECTION IS GOING TO BE DEFINED AT THE BIDI STAGE
 *      I.E. A BIDI CALL WITH "FORCE-LTR" OR "FORCE-RTL" WOULD NOT
 *      USE ICU-BIDI, SOLELY AFFECTING THE HARFBUZZ-DIRECTION FLAG
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"
#include "TextLayout.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 32;

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    
    map<hb_script_t, FontList> fontLists;
    shared_ptr<YFont> font1;
    shared_ptr<YFont> font2;
    shared_ptr<YFont> font3;
    
    TextLayout layout1;
    TextLayout layout2;
    
public:
    void prepareSettings(Settings *settings);
    void setup();
    void draw();
    
    TextLayout createLayout(const TextSpan &span);
    void drawLayout(TextLayout &layout, float y, float left, float right, bool rtl = false);
    void drawHLine(float y);
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1280, 512);
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();
    
    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSans.ttf")), FONT_SIZE, ColorA(1, 1, 0.5f, 1));
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSansHebrew-Regular.ttf")), FONT_SIZE, ColorA(1, 1, 1, 1));
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidNaskh-Regular.ttf")), FONT_SIZE, ColorA(1, 1, 1, 1));
    
    fontLists[HB_SCRIPT_HEBREW].push_back(font2);
    fontLists[HB_SCRIPT_HEBREW].push_back(font1);
    
    fontLists[HB_SCRIPT_ARABIC].push_back(font3);
    fontLists[HB_SCRIPT_ARABIC].push_back(font1);
    
    // ---
    
    layout1 = createLayout(TextSpan("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", HB_SCRIPT_HEBREW, HB_DIRECTION_RTL, "he"));
    layout2 = createLayout(TextSpan("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.", HB_SCRIPT_ARABIC, HB_DIRECTION_RTL, "ar"));
    
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
    
    float left = 24;
    float right = windowSize.x - 24;
    
    // ---
    
    drawLayout(layout1, 128, left, right, true);
    drawLayout(layout2, 256, left, right, false);
}

void Application::drawLayout(TextLayout &layout, float y, float left, float right, bool rtl)
{
    float x = rtl ? (right - layout.advance) : left;
    layout.draw(Vec2f(x, y));
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
}

TextLayout Application::createLayout(const TextSpan &span)
{
    hb_buffer_t *buffer = hb_buffer_create();
    
    TextLayout layout;
    map<uint32_t, Cluster> clusters;
    float combinedAdvance = 0;

    for (auto font : fontLists[span.script])
    {
        span.apply(buffer);
        hb_shape(font->hbFont, buffer, NULL, 0);
        
        auto glyphCount = hb_buffer_get_length(buffer);
        auto glyphInfos = hb_buffer_get_glyph_infos(buffer, NULL);
        auto glyphPositions = hb_buffer_get_glyph_positions(buffer, NULL);
        
        bool hasMissingGlyphs = false;
        
        for (int i = 0; i < glyphCount; i++)
        {
            auto codepoint = glyphInfos[i].codepoint;
            auto cluster = glyphInfos[i].cluster;
            
            auto it = clusters.find(cluster);
            bool clusterFound = (it != clusters.end());
            
            if (codepoint)
            {
                if (clusterFound && (it->second.font != font.get()))
                {
                    continue; // CLUSTER FOUND, WITH ANOTHER FONT (E.G. SPACE)
                }
                else
                {
                    auto offset = Vec2f(glyphPositions[i].x_offset, -glyphPositions[i].y_offset) * font->scale;
                    float advance = glyphPositions[i].x_advance * font->scale.x;
                    
                    if (clusterFound)
                    {
                        it->second.addShape(codepoint, offset, advance);
                    }
                    else
                    {
                        clusters.insert(make_pair(cluster, Cluster(font.get(), codepoint, offset, advance)));
                    }
                    
                    combinedAdvance += advance;
                }
            }
            else if (!clusterFound)
            {
                hasMissingGlyphs = true;
            }
        }
        
        if (!hasMissingGlyphs)
        {
            break; // NO NEED TO PROCEED TO THE NEXT FONT IN THE LIST
        }
        else
        {
            hb_buffer_clear_contents(buffer);
        }
    }
    
    hb_buffer_destroy(buffer);
    
    // ---
    
    if (span.direction == HB_DIRECTION_RTL)
    {
        for (auto it = clusters.rbegin(); it != clusters.rend(); ++it)
        {
            layout.addCluster(it->second);
        }
    }
    else
    {
        for (auto it = clusters.begin(); it != clusters.end(); ++it)
        {
            layout.addCluster(it->second);
        }
    }
    
    return layout;
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
