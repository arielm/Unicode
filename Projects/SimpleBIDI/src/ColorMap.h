/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "hb.h"

#include "cinder/Color.h"

const ci::ColorA COLOR_LTR = ci::ColorA(1, 1, 1, 1);
const ci::ColorA COLOR_RTL = ci::ColorA(1, 1, 0.5f, 1);

class ColorMap
{
public:
    static const ci::ColorA& get(hb_direction_t direction)
    {
        if (direction == HB_DIRECTION_LTR)
        {
            return COLOR_LTR;
        }
        else
        {
            return COLOR_RTL;
        }
    }
};
