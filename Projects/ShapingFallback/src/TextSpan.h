/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "hb.h"

#include <string>

#pragma once

struct TextSpan
{
    std::string text;
    hb_script_t script;
    hb_direction_t direction;
    std::string lang;
    
    TextSpan()
    {}
    
    TextSpan(const std::string &text, hb_script_t script = HB_SCRIPT_LATIN, hb_direction_t direction = HB_DIRECTION_LTR, const std::string &lang = "")
    :
    text(text),
    script(script),
    direction(direction),
    lang(lang)
    {}
};
