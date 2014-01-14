#include "chronotext/android/cinder/CinderRendererJNI.h"

#include "Sketch.h"

chr::CinderDelegate* createDelegate()
{
    chr::CinderDelegate *delegate = new chr::CinderDelegate();
    delegate->sketch = new Sketch(delegate, delegate);

    return delegate;
}

extern "C"
{
    /*
     * COMMENT THE FOLLOWING IF THE "CINDER_APP_NATIVE"
     * MACRO IS USED ELSEWHERE IN THE CODE
     */
    void android_main(struct android_app* state) {}
}
