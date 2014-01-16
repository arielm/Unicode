/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "hb.h"

#include "unicode/unistr.h"

#include <string>

struct TextRun
{
    int32_t start;
    int32_t end;
    
    hb_script_t script;
    std::string language;
    hb_direction_t direction;
    
    TextRun()
    {}
    
    TextRun(int32_t start, int32_t end, hb_script_t script, const std::string &language, hb_direction_t direction)
    :
    start(start),
    end(end),
    script(script),
    language(language),
    direction(direction)
    {}
    
    void apply(const UnicodeString &text, hb_buffer_t *buffer) const
    {
        hb_buffer_clear_contents(buffer);

        hb_buffer_set_script(buffer, script);
        hb_buffer_set_direction(buffer, direction);
        
        if (!language.empty())
        {
            hb_buffer_set_language(buffer, hb_language_from_string(language.data(), -1));
        }
        
        hb_buffer_add_utf16(buffer, text.getBuffer(), text.length(), start, end - start);
    }
};
