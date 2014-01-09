/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * REMINDER: COLORING FOR IS FOR PEDAGOGICAL PURPOSES
 */

#pragma once

#include "cinder/Color.h"

#include <map>

class ColorMap
{
public:
    static ci::ColorA get(const std::string &lang)
    {
        if (lang == "ja")
        {
            return ci::ColorA::hex(0xFF4500);
        }
        if (lang == "ru")
        {
            return ci::ColorA::hex(0xFF8C00);
        }
        if (lang == "ko")
        {
            return ci::ColorA::hex(0xFFD700);
        }
        if (lang == "hi")
        {
            return ci::ColorA::hex(0xFA8072);
        }
        if (lang == "en")
        {
            return ci::ColorA::hex(0xF0E68C);
        }
        if (lang == "th")
        {
            return ci::ColorA::hex(0xBDB76B);
        }
        if (lang == "zh-cn")
        {
            return ci::ColorA::hex(0xFFC0CB);
        }
        if (lang == "ar")
        {
            return ci::ColorA::gray(0.33f);
        }
        if (lang == "he")
        {
            return ci::ColorA::gray(0.5f);
        }
        
        return ci::ColorA::gray(0);
    }
};
