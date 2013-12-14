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
 * 1) COMPARING TWO FONTS
 *
 * 2) TWEAKING THE SCALE OF ONE FONT RELATIVE TO THE OTHER
 *
 *
 * INSTRUCTIONS:
 *
 * 1) DRAG-AND-DROP IN ONE OF THE LEFT OR RIGHT SLOTS:
 *    A) A "DIRECTIVE" XML FILE:
 *       SEE EXAMPLES IN assets/directives
 *    B) A FONT FILE (.TTF OR .OTF):
 *       USING A FONT FROM assets/fonts
 *       OR ANY OTHER FONT ON YOUR SYSTEM
 *
 * 2) CONTROL HORIZONTAL GUIDE-LINE VIA MOUSE-DRAG
 *
 * 3) CHANGE THE FONT-SCALE IN RIGHT-SLOT VIA MOUSE-WHEEL OR THE UP/DOWN KEYS
 *
 * 4) RESET FONT-SCALE BY PRESSING ENTER
 */

#include "cinder/app/AppNative.h"

#include "YFont.h"
#include "Directive.h"

using namespace std;
using namespace ci;
using namespace app;

const float MAX_FONT_SIZE = 96;
const float GUTTER = 24;

const float SCALE_MIN = 0.5f;
const float SCALE_MAX = 2.0f;
const float SCALE_WHEEL_FACTOR = 0.05f;
const float SCALE_KEYBOARD_INCREMENT = 0.01f;

struct Slot
{
    shared_ptr<Directive> directive;
    shared_ptr<YFont> font;
    ShapeLayout lineLayout;
    float scale;
    
    Slot()
    :
    scale(1)
    {}
    
    string getTitle() const
    {
        if (font && directive)
        {
            return font->getName() + " - " + directive->getScriptName() + " [" + toString(scale) + "]";
        }
        else
        {
            return "";
        }
    }
};

class Application : public AppNative
{
    shared_ptr<FreetypeHelper> ftHelper; // THE UNDERLYING FT_Library WILL BE DESTROYED AFTER ALL THE YFont INSTANCES
    
    Slot slot1;
    Slot slot2;
    
    bool mousePressed;
    float mouseY;
    
public:
    Application()
    :
    mousePressed(false)
    {}

    void prepareSettings(Settings *settings);
    void setup();
    
    void update();
    void draw();
    
    void drawSlot(const Slot &slot, float y, float left, float right, float zoom);
    void drawVLine(float x, float top = numeric_limits<float>::min(), float bottom = numeric_limits<float>::max());
    void drawHLine(float y, float left = numeric_limits<float>::min(), float right = numeric_limits<float>::max());
    
    void mouseDown(MouseEvent event);
    void mouseDrag(MouseEvent event);
    void mouseUp(MouseEvent event);
    void mouseWheel(MouseEvent event);
    void keyDown(KeyEvent event);
    void fileDrop(FileDropEvent event);
    
    void applyDirective(Slot &slot, shared_ptr<Directive> directive);
    void updateTitle();
    float getSlotZoom(const Slot &slot, float left, float right);
};

void Application::prepareSettings(Settings *settings)
{
    settings->setWindowSize(1280, 360);
}

void Application::setup()
{
    ftHelper = make_shared<FreetypeHelper>();
    
    applyDirective(slot1, make_shared<Directive>(TextSpan("drop directive or font")));
    applyDirective(slot2, make_shared<Directive>(TextSpan("drop directive or font")));
    
//  applyDirective(slot1, make_shared<Directive>(TextSpan("ABCDE abcde 0123456789"), "fonts/DroidSans.ttf"));
//  applyDirective(slot2, make_shared<Directive>(TextSpan("אבגדה 0123456789", HB_SCRIPT_HEBREW, HB_DIRECTION_RTL), "fonts/DroidSansHebrew-Regular.ttf"));
    
//  applyDirective(slot1, make_shared<Directive>(loadAsset("directives/Hebrew1.xml")));
//  applyDirective(slot2, make_shared<Directive>(loadAsset("directives/Hebrew1_osx.xml")));
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Application::update()
{
    updateTitle();
}

void Application::draw()
{
    gl::clear(Color::gray(0.5f), false);
    
    Vec2i windowSize = toPixels(getWindowSize());
    gl::setMatricesWindow(windowSize, true);
    
    // ---
    
    float y = windowSize.y * 0.5f;
    float middle = windowSize.x * 0.5f;
    
    float left1 = GUTTER;
    float left2 = middle + GUTTER * 0.5f;
    
    float right1 = middle - GUTTER * 0.5f;
    float right2 = windowSize.x - GUTTER;
    
    float zoom = min(getSlotZoom(slot1, left1, right1), getSlotZoom(slot2, left2, right2));
    
    drawSlot(slot1, y, left1, right1, zoom);
    drawSlot(slot2, y, left2, right2, zoom);
    
    // ---
    
    if (mousePressed)
    {
        glColor4f(0, 0.5f, 1, 0.75f);
        drawHLine(mouseY);
    }
}

void Application::drawSlot(const Slot &slot, float y, float left, float right, float zoom)
{
    float x = (slot.lineLayout.direction == HB_DIRECTION_LTR) ? left : right;
    
    glColor4f(1, 1, 1, 1);
    slot.font->drawLayout(slot.lineLayout, Vec2f(x, y), slot.scale * zoom);
    
    glColor4f(1, 0.75f, 0, 0.5f);
    drawHLine(y, left, right);
    
    glColor4f(1, 1, 0, 0.33f);
    drawHLine(y - slot.font->ascent, left, right);
    drawHLine(y + slot.font->descent, left, right);

    glColor4f(1, 0.25f, 0, 0.33f);
    drawVLine(left);
    drawVLine(right);
}

void Application::drawVLine(float x, float top, float bottom)
{
    gl::drawLine(Vec2f(x, top), Vec2f(x, bottom));
}

void Application::drawHLine(float y, float left, float right)
{
    gl::drawLine(Vec2f(left, y), Vec2f(right, y));
}

void Application::mouseDown(MouseEvent event)
{
    mousePressed = true;
    mouseY = event.getY();
}

void Application::mouseDrag(MouseEvent event)
{
    mouseY = event.getY();
}

void Application::mouseUp(MouseEvent event)
{
    mousePressed = false;
}

void Application::mouseWheel(MouseEvent event)
{
    slot2.scale += event.getWheelIncrement() * math<float>::exp(slot2.scale) * SCALE_WHEEL_FACTOR;
    slot2.scale = math<float>::clamp(slot2.scale, SCALE_MIN, SCALE_MAX);
}

void Application::keyDown(KeyEvent event)
{
    switch (event.getCode())
    {
        case KeyEvent::KEY_RETURN:
            slot2.scale = 1;
            break;
            
        case KeyEvent::KEY_UP:
            slot2.scale = math<float>::clamp(slot2.scale + SCALE_KEYBOARD_INCREMENT, SCALE_MIN, SCALE_MAX);
            break;
            
        case KeyEvent::KEY_DOWN:
            slot2.scale = math<float>::clamp(slot2.scale - SCALE_KEYBOARD_INCREMENT, SCALE_MIN, SCALE_MAX);
            break;
    }
}

void Application::fileDrop(FileDropEvent event)
{
    if (event.getNumFiles() == 1)
    {
        auto file = event.getFile(0);
        auto extension = file.extension();
        
        if (!is_directory(file))
        {
            bool isLeft = (event.getX() < toPixels(getWindowWidth()) / 2);
            Slot &slot = isLeft ? slot1 : slot2;
            
            if (extension == ".xml")
            {
                try
                {
                    applyDirective(slot, make_shared<Directive>(loadFile(file)));
                }
                catch (exception &e)
                {
                    applyDirective(slot, make_shared<Directive>(e));
                }
            }
            else if ((extension == ".ttf") || (extension == ".otf"))
            {
                applyDirective(slot, make_shared<Directive>(file, *slot.directive));
            }
        }
    }
}

void Application::applyDirective(Slot &slot, shared_ptr<Directive> directive)
{
    slot.directive = directive;
    slot.font = make_shared<YFont>(ftHelper, directive->fontPath, MAX_FONT_SIZE);
    
    slot.lineLayout = slot.font->createLayout(directive->span);
}

void Application::updateTitle()
{
    getWindow()->setTitle(slot1.getTitle() + "  |  " + slot2.getTitle());
}

float Application::getSlotZoom(const Slot &slot, float left, float right)
{
    float width = right - left;
    
    if (slot.lineLayout.advance * slot.scale > width)
    {
        return width / slot.lineLayout.advance / slot.scale;
    }
    else
    {
        return 1;
    }
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_MSAA_4))
