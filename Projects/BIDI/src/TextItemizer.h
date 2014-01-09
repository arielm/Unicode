/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "LanguageHelper.h"
#include "TextGroup.h"

class TextItemizer
{
public:
    TextGroup process(const std::string &input, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_LTR);
    
protected:
    LanguageHelper languageHelper;
};
