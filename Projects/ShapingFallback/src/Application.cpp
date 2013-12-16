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
    void drawSpan(const YFont &font1, const YFont &font2, const TextSpan &span, float y);
    void drawSpan(const YFont &font, const TextSpan &span, float x, float y);
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
    
    drawSpan(*font1, *font2, TextSpan("אבגדה 0123456789", HB_SCRIPT_HEBREW, HB_DIRECTION_RTL, "he"), 256);
}

void Application::drawSpan(const YFont &font1, const YFont &font2, const TextSpan &span, float y)
{
    glColor4f(1, 1, 1, 1);
    drawSpan(font1, span, 24, y);
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
    
    glColor4f(1, 1, 0, 0.33f);
    drawHLine(y - font1.ascent);
    drawHLine(y + font1.descent);
}

void Application::drawSpan(const YFont &font, const TextSpan &span, float x, float y)
{
    hb_buffer_t *hbBuffer = hb_buffer_create();
    
    hb_buffer_set_direction(hbBuffer, span.direction);
    hb_buffer_set_script(hbBuffer, span.script);
    
    if (!span.lang.empty())
    {
        hb_buffer_set_language(hbBuffer, hb_language_from_string(span.lang.data(), -1));
    }
    
    size_t textSize = span.text.size();
    hb_buffer_add_utf8(hbBuffer, span.text.data(), textSize, 0, textSize);
    hb_shape(font.hbFont, hbBuffer, NULL, 0);
    
    unsigned int glyphCount;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(hbBuffer, &glyphCount);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(hbBuffer, &glyphCount);
    
    // ---
    
    glPushMatrix();
    glTranslatef(x, y, 0);
    
    for (int i = 0; i < glyphCount; i++)
    {
        const hb_glyph_position_t &pos = glyph_pos[i];
        Vec2f offset(pos.x_offset, -pos.y_offset);
        Vec2f advance(pos.x_advance, pos.y_advance);
        
        YGlyph *glyph = font.createGlyph(glyph_info[i].codepoint);
        
        if (glyph)
        {
            if (glyph->texture)
            {
                gl::draw(glyph->texture, glyph->offset + offset * font.scale);
            }
            
            delete glyph;
        }
        
        gl::translate(advance * font.scale);
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
