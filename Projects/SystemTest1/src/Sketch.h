#pragma once

#include "chronotext/cinder/CinderSketch.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch(void *context, void *delegate = NULL)
    :
    chr::CinderSketch(context, delegate)
    {}

    void setup(bool renew);
    void draw();
};
