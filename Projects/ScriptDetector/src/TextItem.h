/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "hb.h"

#include <string>

struct TextItem
{
    int32_t start;
    int32_t end;
    
    hb_script_t script;
    std::string lang;
    hb_direction_t direction;
    
    TextItem(int32_t start, int32_t end, hb_script_t script, const std::string &lang, hb_direction_t direction)
    :
    start(start),
    end(end),
    script(script),
    lang(lang),
    direction(direction)
    {}
};
