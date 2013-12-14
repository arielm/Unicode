/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"
#include "Directive.h"

using namespace std;
using namespace ci;
using namespace app;

const float FONT_SIZE = 48;
const float LINE_TOP = 90;
const float LINE_SPACING = 90;

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    
    shared_ptr<Directive> currentDirective;
    shared_ptr<YFont> currentFont;
    vector<ShapeLayout> lineLayouts;
   
public:
    void prepareSettings(Settings *settings);
    void setup();
    
    void draw();
    void drawLineLayout(YFont &font, const ShapeLayout &layout, float y, float left, float right);
    void drawHLine(float y);
    
    void fileDrop(FileDropEvent event);
    void applyDirective(shared_ptr<Directive> directive);
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1280, 720);
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();
    
    applyDirective(make_shared<Directive>("drop directive or font"));
//  applyDirective(make_shared<Directive>(loadAsset("directives/Hebrew1_osx.xml")));
    
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
        drawLineLayout(*currentFont, layout, y, left, right);
        y += LINE_SPACING;
    }
}

void Application::drawLineLayout(YFont &font, const ShapeLayout &layout, float y, float left, float right)
{
    float x = (layout.direction == HB_DIRECTION_LTR) ? left : right;
    
    glColor4f(1, 1, 1, 1);
    font.drawLayout(layout, Vec2f(x, y));
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y);
    
    glColor4f(1, 1, 0, 0.33f);
    drawHLine(y - font.ascent);
    drawHLine(y + font.descent);
}

void Application::drawHLine(float y)
{
    gl::drawLine(Vec2f(-9999, y), Vec2f(+9999, y));
}

void Application::fileDrop(FileDropEvent event)
{
    if (event.getNumFiles() == 1)
    {
        auto file = event.getFile(0);
        auto extension = file.extension();
        
        if (!is_directory(file))
        {
            if (extension == ".xml")
            {
                try
                {
                    applyDirective(make_shared<Directive>(loadFile(file)));
                }
                catch (exception &e)
                {
                    applyDirective(make_shared<Directive>(e));
                }
            }
            else if ((extension == ".ttf") || (extension == ".otf"))
            {
                applyDirective(make_shared<Directive>(file, *currentDirective));
            }
        }
    }
}

void Application::applyDirective(shared_ptr<Directive> directive)
{
    currentDirective = directive;
    currentFont = make_shared<YFont>(ftHelper, directive->fontPath, FONT_SIZE);
    
    lineLayouts.clear();

    for (auto line : directive->lines)
    {
        lineLayouts.push_back(currentFont->createLayout(TextSpan(line, directive->script, directive->direction)));
    }
    
    getWindow()->setTitle(currentFont->getName() + " - " + directive->getScriptName());
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
