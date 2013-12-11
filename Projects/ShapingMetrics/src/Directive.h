/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "TextSpan.h"

#include "cinder/Xml.h"

const hb_script_t DEFAULT_SCRIPT = HB_SCRIPT_LATIN;
const hb_direction_t DEFAULT_DIRECTION = HB_DIRECTION_LTR;

const std::string DEFAULT_SCRIPT_TAG = "Latn";
const std::string DEFAULT_FONT_PATH = "/Library/Fonts/Arial Unicode.ttf"; // OSX

class Directive
{
public:
    TextSpan span;
    ci::fs::path fontPath;
    
    Directive(const std::string &text, const ci::fs::path &fontPath = DEFAULT_FONT_PATH, hb_script_t script = DEFAULT_SCRIPT, hb_direction_t direction = DEFAULT_DIRECTION);
    Directive(ci::DataSourceRef source);
    Directive(const std::exception &e);
    Directive(const ci::fs::path &fontPath, const Directive &baseDirective);
    
protected:
    ci::fs::path getFontPath(const ci::fs::path virtualPath);
    std::string getText(const std::string &text);
};
