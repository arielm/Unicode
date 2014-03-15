/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "chronotext/cinder/CinderApp.h"
#include "chronotext/utils/Utils.h"

#include "Sketch.h"

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;
using namespace zf;

class Application : public CinderApp
{
    Sketch *target;
    
public:
    Application();
    void prepareSettings(Settings *settings);
    void keyDown(KeyEvent event);
};

Application::Application()
{
    sketch = target = new Sketch(this);
}

void Application::prepareSettings(Settings *settings)
{
#if defined(CINDER_MAC) || defined(CINDER_MSW)
    settings->setWindowSize(1024, 736);
    settings->enableHighDensityDisplay();
    settings->disableFrameRate();
#endif
}

void Application::keyDown(KeyEvent event)
{
    switch (event.getCode())
    {
        case KeyEvent::KEY_u:
            target->fontManager.unload();
            break;
            
        case KeyEvent::KEY_RETURN:
            target->shuffleLines();
            break;

        case KeyEvent::KEY_SPACE:
            target->oscillate ^= true;
            break;
            
        case KeyEvent::KEY_t:
            target->align = ZFont::ALIGN_TOP;
            break;
            
        case KeyEvent::KEY_m:
            target->align = ZFont::ALIGN_MIDDLE;
            break;

        case KeyEvent::KEY_b:
            target->align = ZFont::ALIGN_BOTTOM;
            break;
            
        case KeyEvent::KEY_x:
            cout << "TEXTURE MEMORY USAGE: " << prettyBytes(target->fontManager.getTextureMemoryUsage()) << endl;
            break;
            
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
    }
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
