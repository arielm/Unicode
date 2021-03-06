/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "chronotext/cinder/CinderApp.h"

#include "Sketch.h"

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;

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
            
        case KeyEvent::KEY_r:
            target->fontManager.reload();
            break;
    }
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
