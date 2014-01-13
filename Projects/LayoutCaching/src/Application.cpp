/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * DONE:
 *
 * 1) LayoutCache IN PLACE:
 *    - ADAPTED TO TextLine
 *    - LRU CACHE STRATEGY BASED ON boost::bimaps
 *    - TESTED ON iOS AND ANDROID:
 *      - LIKE IN SimpleRendering PROJECT:
 *        CRASH WHEN RETURNING TO APP ON XOOM 1 (ANDROID 3)
 */

/*
 * TODO:
 *
 * 1) ANDROID CRASH WHEN RETURNING TO APP ON XOOM 1:
 *    SEE IF IT OCCURS WHEN USING new-chronotext-toolkit
 *    (I.E. INSTEAD OF USING A SOLUTION BASED ON NativeActivity)
 *
 * 2) ADJUST FONTS:
 *    - iOS:
 *      - "Geeza Pro" SHOULD BE REPLACED (ARABIC GLYPHS ARE NOT JOINED)
 *    - ANDROID:
 *      - HEBREW FONT IS MISSING
 *      - ARABIC FONT IS PROBABLY OUTDATED
 *      - THAI FONT IS BUGGY...
 *
 * 3) ADD SCALE-FACTOR FOR ACTUAL-FONTS IN XML DEFINITION:
 *    - NECESSARY IN CASE WE NEED TO MATCH SIZES BETWEEN "Geeza Pro" (SMALLER?) AND "Arial" (BIGGER?)
 */

#include "chronotext/cinder/CinderApp.h"

#include "Sketch.h"

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;

class Application : public CinderApp
{
public:
    Application();
    void prepareSettings(Settings *settings);
};

Application::Application()
{
    sketch = new Sketch(this);
}

void Application::prepareSettings(Settings *settings)
{
#if defined(CINDER_MAC) || defined(CINDER_MSW)
    settings->setWindowSize(1024, 736);
    settings->enableHighDensityDisplay();
    settings->disableFrameRate();
#endif
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
