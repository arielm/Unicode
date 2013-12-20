/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"
#include "TextGroup.h"
#include "TextLayout.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 32;

typedef vector<shared_ptr<YFont>> FontList;

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    
    map<hb_script_t, FontList> fontLists;
    shared_ptr<YFont> font1;
    shared_ptr<YFont> font2;
    shared_ptr<YFont> font3;
    
    TextLayout layout1;
    TextLayout layout2;
    TextLayout layout3;
    TextLayout layout4;
    TextLayout layout5;
    TextLayout layout6;
    TextLayout layout7;
    
public:
    void prepareSettings(Settings *settings);
    void setup();
    void draw();
    
    TextLayout createLayout(const vector<TextSpan> &spans);
    void drawLayout(TextLayout &layout, float y, float left, float right, bool rtl = false);
    void drawHLine(float y);
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1280, 720);
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

    layout1 = createLayout(TextGroup("The title is مفتاح معايير الويب in Arabic.", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR).runs);
    layout2 = createLayout(TextGroup("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR).runs);
    layout3 = createLayout(TextGroup("The names of these states in Arabic are مصر,‎ البحرين and الكويت respectively.", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR).runs);
    layout4 = createLayout(TextGroup("W3C‏ (World Wide Web Consortium) מעביר את שירותי הארחה באירופה ל - ERCIM.", HB_SCRIPT_HEBREW, "he", HB_DIRECTION_RTL).runs);
    layout5 = createLayout(TextGroup("The title says \"W3C, פעילות הבינאום\" in Hebrew.", HB_SCRIPT_HEBREW, "he", HB_DIRECTION_LTR).runs);
    layout6 = createLayout(TextGroup("one two ثلاثة four خمسة", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR).runs);
    layout7 = createLayout(TextGroup("one two ثلاثة 1234 خمسة", HB_SCRIPT_ARABIC, "ar", HB_DIRECTION_LTR).runs);
    
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
    
    drawLayout(layout1, 90, left, right, false);
    drawLayout(layout2, 180, left, right, false);
    drawLayout(layout3, 270, left, right, false);
    drawLayout(layout4, 360, left, right, true);
    drawLayout(layout5, 450, left, right, false);
    drawLayout(layout6, 540, left, right, false);
    drawLayout(layout7, 630, left, right, false);
}

void Application::drawLayout(TextLayout &layout, float y, float left, float right, bool rtl)
{
    float x = rtl ? (right - layout.advance) : left;
    layout.draw(Vec2f(x, y));
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
}

TextLayout Application::createLayout(const vector<TextSpan> &spans)
{
    hb_buffer_t *buffer = hb_buffer_create();
    
    TextLayout layout;
    float combinedAdvance = 0;
    map<uint32_t, Cluster> clusters;
    
    for (auto span : spans)
    {
        clusters.clear();

        for (auto font : fontLists[span.script])
        {
            hb_buffer_clear_contents(buffer);
            
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
        }
        
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
    }
    
    hb_buffer_destroy(buffer);
    
    return layout;
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
