/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "VirtualFont.h"
#include "Timing.h"

#define SHAPING_COUNT 100

class Test
{
public:
    static void measureShaping(std::ostream &output, VirtualFont *virtualFont, const std::vector<TextRun> &runs)
    {
        Timing t(output, "SHAPING");
        
        for (int i = 0; i < SHAPING_COUNT; i++)
        {
            for (auto run : runs)
            {
                std::unique_ptr<TextLayout>(virtualFont->createTextLayout(run));
            }
        }
    }
};
