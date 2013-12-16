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
 * DroidSansHebrew-Regular.ttf IS ONLY CONTAINING HEBREW CHARACTERS AND DIACRITICS
 * THEREFORE, WE RELY ON DroidSans.ttf FOR ANYTHING ELSE
 *
 *
 * UPDATE:
 * - SOME PROGRESS WITH CLUSTERS AND DIACRITICS, BUT STILL NOT THERE
 *   - std::multimap IS NOT THE RIGHT CONTAINER:
 *     DIACRITICS ARE PLACE AFTER THE LETTER,
 *     WHICH IS PROBLEMATIC WITH THE CURRENT ADVANCEMENT METHOD
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 56;

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    
    shared_ptr<YFont> font1;
    shared_ptr<YFont> font2;
   
public:
    void prepareSettings(Settings *settings);
    void setup();
    
    void draw();
    void drawSpan(YFont &font1, YFont &font2, const TextSpan &span, float y);
    void drawSpan(YFont &font1, YFont &font2, const TextSpan &span, float x, float y);
    void drawHLine(float y);
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1024, 512);
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();

    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSansHebrew-Regular.ttf")), FONT_SIZE, ColorA(1, 1, 1, 1));
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSans.ttf")), FONT_SIZE, ColorA(1, 1, 0.5f, 1));
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
    gl::setMatricesWindow(toPixels(getWindowSize()), true);

    drawSpan(*font1, *font2, TextSpan("וְהָהַר, מַהוּ לַזֵּה? – זֹאת הִיא הַשְּׁאֵלָה.", HB_SCRIPT_HEBREW, HB_DIRECTION_RTL, "he"), 256);
}

void Application::drawSpan(YFont &font1, YFont &font2, const TextSpan &span, float y)
{
    drawSpan(font1, font2, span, 24, y);
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
}

void Application::drawSpan(YFont &font1, YFont &font2, const TextSpan &span, float x, float y)
{
    const char *shapers[]  = { "ot", "fallback", NULL };
    hb_buffer_t *buffer = hb_buffer_create();

    multimap<uint32_t, Shape> shapes;

    /*
     * FIRST PASS
     */

    span.apply(buffer);
    hb_shape_full(font1.hbFont, buffer, NULL, 0, shapers);
    
    auto glyphCount = hb_buffer_get_length(buffer);
    auto glyph_info = hb_buffer_get_glyph_infos(buffer, nullptr);
    auto glyph_pos = hb_buffer_get_glyph_positions(buffer, nullptr);
    
    for (int i = 0; i < glyphCount; i++)
    {
        auto codepoint = glyph_info[i].codepoint;
        Vec2f offset(glyph_pos[i].x_offset, -glyph_pos[i].y_offset);
        float advance = glyph_pos[i].x_advance;
        
        if (codepoint)
        {
            shapes.emplace(glyph_info[i].cluster, Shape(&font1, codepoint, offset, advance));
        }
        
//      cout << codepoint << " | " << glyph_info[i].cluster << " | " << advance * font1.scale.x << endl;
    }
//  cout << endl;

    hb_buffer_clear_contents(buffer);

    /*
     * SECOND PASS
     */
    
    span.apply(buffer);
    hb_shape_full(font2.hbFont, buffer, NULL, 0, shapers);
    
    glyphCount = hb_buffer_get_length(buffer);
    glyph_info = hb_buffer_get_glyph_infos(buffer, nullptr);
    glyph_pos = hb_buffer_get_glyph_positions(buffer, nullptr);
    
    for (int i = 0; i < glyphCount; i++)
    {
        auto codepoint = glyph_info[i].codepoint;
        Vec2f offset(glyph_pos[i].x_offset, -glyph_pos[i].y_offset);
        float advance = glyph_pos[i].x_advance;
        
        if (codepoint)
        {
            shapes.emplace(glyph_info[i].cluster, Shape(&font2, codepoint, offset, advance));
        }
        
//      cout << codepoint << " | " << glyph_info[i].cluster << " | " << advance * font2.scale.x << endl;
    }
//  cout << endl;
    
    hb_buffer_destroy(buffer);
    
    // ---
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    
    for (auto it = shapes.rbegin(); it != shapes.rend(); ++it)
    {
        float advance = it->second.draw();
        glTranslatef(advance, 0, 0);
    }
    
    glPopMatrix();
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
