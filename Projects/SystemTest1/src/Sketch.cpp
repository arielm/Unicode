#include "Sketch.h"

#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;
using namespace ci::app;
using namespace chr;

void Sketch::setup(bool renew)
{}

void Sketch::draw()
{
    gl::setMatricesWindow(getWindowSize());
    gl::clear(Color(1.0f, 0.5f, 0.0f), false);
}
