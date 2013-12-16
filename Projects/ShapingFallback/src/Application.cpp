/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * DEMONSTRATING SHAPING FALLBACK, AS TAKING PLACE IN:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/include/mapnik/text/harfbuzz_shaper.hpp
 *
 *
 * DroidSansHebrew-Regular.ttf IS ONLY CONTAINING HEBREW CHARACTERS AND DIACRITICS
 * THEREFORE, WE RELY ON DroidSans.ttf FOR ANYTHING ELSE
 *
 *
 * KEEPING THE CODE DELIBERATELY NOT OPTIMIZED TO FACILITATE UNDERSTANDING
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
    
    drawSpan(*font1, *font2, TextSpan("אֱב;גד 123וּ", HB_SCRIPT_HEBREW, HB_DIRECTION_RTL, "he"), 256);
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

    vector<Shape> shapes;

    /*
     * FIRST PASS
     */

    span.apply(buffer);
    hb_shape_full(font1.hbFont, buffer, NULL, 0, shapers);
    
    unsigned int glyphCount;
    auto glyph_info = hb_buffer_get_glyph_infos(buffer, &glyphCount);
    auto glyph_pos = hb_buffer_get_glyph_positions(buffer, nullptr);
    
    for (int i = 0; i < glyphCount; i++)
    {
        auto codepoint = glyph_info[i].codepoint;
        Vec2f offset(glyph_pos[i].x_offset, -glyph_pos[i].y_offset);
        float advance = glyph_pos[i].x_advance;
        
        if (codepoint)
        {
            shapes.emplace_back(&font1, codepoint, offset, advance);
        }
        else
        {
            shapes.emplace_back(); // MAKING ROOM FOR LATER
        }
    }

    hb_buffer_clear_contents(buffer);

    /*
     * SECOND PASS
     */
    
    span.apply(buffer);
    hb_shape_full(font2.hbFont, buffer, NULL, 0, shapers);
    
    for (int i = 0; i < glyphCount; i++)
    {
        auto codepoint = glyph_info[i].codepoint;
        Vec2f offset(glyph_pos[i].x_offset, -glyph_pos[i].y_offset);
        float advance = glyph_pos[i].x_advance;
        
        if (codepoint)
        {
            shapes[i].update(&font2, codepoint, offset, advance);
        }
    }
    
    hb_buffer_destroy(buffer);
    
    // ---
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    
    for (int i = 0; i < glyphCount; i++)
    {
        float advance = shapes[i].draw();
        glTranslatef(advance, 0, 0);
    }
    
    glPopMatrix();
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))