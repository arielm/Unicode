/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * INVESTIGATING "FALLBACK", AS TAKING PLACE IN:
 * https://github.com/mapnik/mapnik/blob/master/include/mapnik/text/harfbuzz_shaper.hpp#L61
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

    font1 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSansHebrew-Regular.ttf")), FONT_SIZE);
    font2 = make_shared<YFont>(ftHelper, FontDescriptor(loadAsset("fonts/DroidSans.ttf")), FONT_SIZE);
    
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
    
    drawSpan(*font1, *font2, TextSpan("אב;גד 123ה", HB_SCRIPT_HEBREW, HB_DIRECTION_RTL, "he"), 256);
}

void Application::drawSpan(YFont &font1, YFont &font2, const TextSpan &span, float y)
{
    glColor4f(1, 1, 1, 1);
    drawSpan(font1, font2, span, 24, y);
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
    
    glColor4f(1, 1, 0, 0.33f);
    drawHLine(y - font1.ascent);
    drawHLine(y + font1.descent);
}

void Application::drawSpan(YFont &font1, YFont &font2, const TextSpan &span, float x, float y)
{
    const char *shapers[]  = { "ot", "fallback", NULL };
    hb_buffer_t *hbBuffer = hb_buffer_create();
    
    size_t textSize = span.text.size();
    vector<Shape> shapes;

    /*
     * FIRST PASS
     */

    hb_buffer_set_direction(hbBuffer, span.direction);
    hb_buffer_set_script(hbBuffer, span.script);
    
    if (!span.lang.empty())
    {
        hb_buffer_set_language(hbBuffer, hb_language_from_string(span.lang.data(), -1));
    }
    
    hb_buffer_add_utf8(hbBuffer, span.text.data(), textSize, 0, textSize);
    hb_shape_full(font1.hbFont, hbBuffer, NULL, 0, shapers);
    
    unsigned int glyphCount;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(hbBuffer, &glyphCount);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(hbBuffer, nullptr);
    
    for (int i = 0; i < glyphCount; i++)
    {
        auto codepoint = glyph_info[i].codepoint;
        auto cluster = glyph_info[i].cluster;
        
        const hb_glyph_position_t &pos = glyph_pos[i];
        Vec2f offset(pos.x_offset, -pos.y_offset);
        float advance = pos.x_advance;
        
        if (codepoint)
        {
            shapes.emplace_back(&font1, codepoint, offset, advance);
        }
        else
        {
            shapes.emplace_back(cluster);
        }
        
//      cout << codepoint << " | " << cluster << " | " << advance * font1.scale.x << endl;
    }

    /*
     * SECOND PASS
     */
    
    hb_buffer_clear_contents(hbBuffer);
    
    hb_buffer_set_direction(hbBuffer, span.direction);
    hb_buffer_set_script(hbBuffer, span.script);
    
    if (!span.lang.empty())
    {
        hb_buffer_set_language(hbBuffer, hb_language_from_string(span.lang.data(), -1));
    }
    
    hb_buffer_add_utf8(hbBuffer, span.text.data(), textSize, 0, textSize);
    hb_shape_full(font2.hbFont, hbBuffer, NULL, 0, shapers);
    
    for (int i = 0; i < glyphCount; i++)
    {
        auto codepoint = glyph_info[i].codepoint;
        
        const hb_glyph_position_t &pos = glyph_pos[i];
        Vec2f offset(pos.x_offset, -pos.y_offset);
        float advance = pos.x_advance;
        
        if (codepoint)
        {
            shapes[i].update(&font2, codepoint, offset, advance);
        }
        
//      cout << codepoint << " | " << cluster << " | " << advance * font2.scale.x << endl;
    }
    
    // ---
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    
    for (int i = 0; i < glyphCount; i++)
    {
        float advance = shapes[i].draw();
        glTranslatef(advance, 0, 0);
    }
    
    glPopMatrix();
    
    // ---
    
    hb_buffer_destroy(hbBuffer);
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
