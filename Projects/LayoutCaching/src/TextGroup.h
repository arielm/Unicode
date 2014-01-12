/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "TextItem.h"

#include <vector>

struct TextGroup
{
    UnicodeString text;
    hb_direction_t overallDirection;
    std::vector<TextItem> items;
    
    TextGroup(const std::string &input, hb_direction_t overallDirection = HB_DIRECTION_LTR)
    :
    overallDirection(overallDirection)
    {
        text = UnicodeString::fromUTF8(input);
    }
    
    void addItem(int32_t start, int32_t end, hb_script_t script, const std::string &lang, hb_direction_t direction)
    {
        items.emplace_back(start, end, script, lang, direction);
    }
};
