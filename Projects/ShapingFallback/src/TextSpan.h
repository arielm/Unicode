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

class TextSpan
{
public:
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
    
    void apply(hb_buffer_t *buffer) const
    {
        hb_buffer_set_direction(buffer, direction);
        hb_buffer_set_script(buffer, script);
        
        if (!lang.empty())
        {
            hb_buffer_set_language(buffer, hb_language_from_string(lang.data(), -1));
        }
        
        auto textSize = text.size();
        hb_buffer_add_utf8(buffer, text.data(), textSize, 0, textSize);
    }
};
