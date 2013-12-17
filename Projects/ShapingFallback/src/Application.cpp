/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * TESTING SHAPING FALLBACK, AS TAKING PLACE IN:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/include/mapnik/text/harfbuzz_shaper.hpp
 *
 *
 * DroidSansHebrew-Regular.ttf IS ONLY CONTAINING HEBREW LETTERS AND DIACRITICS
 * THEREFORE, WE RELY ON DroidSans.ttf FOR ANYTHING ELSE
 *
 *
 * UPDATE:
 * - INTRODUCING FONT-LISTS
 * - DRAWING RTL SPANS FROM THE RIGHT
 * - "PASSES MERGED"
 * - PROBLEM: DIACRITICS ARE MIS-POSITIONED IN THAI AND HINDI!
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"
#include "Cluster.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 56;

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    
    shared_ptr<YFont> font1;
    shared_ptr<YFont> font2;
    shared_ptr<YFont> font3;
    shared_ptr<YFont> font4;
    
    FontList fontList1;
    FontList fontList2;
    FontList fontList3;
    
public:
    void prepareSettings(Settings *settings);
    void setup();
    
    void draw();
    void drawSpan(const FontList &fontList, const TextSpan &span, float y, float left, float right);
    void drawSpan(const FontList &fontList, const TextSpan &span, float x, float y);
    void drawHLine(float y);
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1024, 512);
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();

    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSans.ttf")), FONT_SIZE, ColorA(1, 1, 0.5f, 1));
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSansHebrew-Regular.ttf")), FONT_SIZE, ColorA(1, 1, 1, 1));
    font3 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSansThai.ttf")), FONT_SIZE, ColorA(1, 1, 1, 1));
    font4 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSansDevanagari-Regular.ttf")), FONT_SIZE, ColorA(1, 1, 1, 1));
    
    fontList1.push_back(font2);
    fontList1.push_back(font1);
    
    fontList2.push_back(font3);
    fontList2.push_back(font1);
    
    fontList3.push_back(font4);
    fontList3.push_back(font1);
    
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

    drawSpan(fontList1, TextSpan("וְהָהַר, מַהוּ לַזֵּה? – זֹאת הִיא הַשְּׁאֵלָה.", HB_SCRIPT_HEBREW, HB_DIRECTION_RTL, "he"), 128, left, right); // http://goo.gl/M1Z8kl
    drawSpan(fontList2, TextSpan("Unicode คืออะไร?", HB_SCRIPT_THAI, HB_DIRECTION_LTR, "th"), 256, left, right); // http://www.unicode.org/standard/translations/thai.html
    drawSpan(fontList3, TextSpan("यूनिकोड क्या है?", HB_SCRIPT_DEVANAGARI, HB_DIRECTION_LTR, "hi"), 384, left, right); // http://www.unicode.org/standard/translations/hindi.html
}

void Application::drawSpan(const FontList &fontList, const TextSpan &span, float y, float left, float right)
{
    float x = (span.direction == HB_DIRECTION_LTR) ? left : right;
    
    drawSpan(fontList, span, x, y);
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
}

void Application::drawSpan(const FontList &fontList, const TextSpan &span, float x, float y)
{
    hb_buffer_t *buffer = hb_buffer_create();
    
    map<uint32_t, Cluster> clusters;
    float combinedAdvance = 0;
    
    for (auto font : fontList)
    {
        span.apply(buffer);
        hb_shape(font->hbFont, buffer, NULL, 0);
        
        auto glyphCount = hb_buffer_get_length(buffer);
        auto glyphInfos = hb_buffer_get_glyph_infos(buffer, nullptr);
        auto glyphPositions = hb_buffer_get_glyph_positions(buffer, nullptr);
        
        bool hasMissingGlyphs = false;
        
        for (int i = 0; i < glyphCount; i++)
        {
            auto codepoint = glyphInfos[i].codepoint;
            auto cluster = glyphInfos[i].cluster;
            
            auto it = clusters.find(cluster);
            bool clusterExists = (it != clusters.end());
            
            if (codepoint)
            {
                if (clusterExists && (it->second.font != font.get()))
                {
                    continue; // CLUSTER ALREADY DEFINED WITH ANOTHER FONT, E.G. SPACE
                }
                else
                {
                    auto offset = Vec2f(glyphPositions[i].x_offset, -glyphPositions[i].y_offset) * font->scale;
                    float advance = glyphPositions[i].x_advance * font->scale.x;
                    
                    if (clusterExists)
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
            else if (!clusterExists)
            {
                hasMissingGlyphs = true;
            }
        }
        
        if (!hasMissingGlyphs)
        {
            break;
        }
        else
        {
            hb_buffer_clear_contents(buffer);
        }
    }
    
    hb_buffer_destroy(buffer);

    // ---
    
    glPushMatrix();
    
    if (span.direction == HB_DIRECTION_RTL)
    {
        glTranslatef(x - combinedAdvance, y, 0);
        
        for (auto it = clusters.rbegin(); it != clusters.rend(); ++it)
        {
            float advance = it->second.draw();
            glTranslatef(advance, 0, 0);
        }
    }
    else
    {
        glTranslatef(x, y, 0);
        
        for (auto it = clusters.begin(); it != clusters.end(); ++it)
        {
            float advance = it->second.draw();
            glTranslatef(advance, 0, 0);
        }
    }
    
    glPopMatrix();
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
