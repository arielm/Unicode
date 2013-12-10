/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Xml.h"

#include "hb.h"

const hb_script_t DEFAULT_SCRIPT = HB_SCRIPT_LATIN;
const hb_direction_t DEFAULT_DIRECTION = HB_DIRECTION_LTR;

const std::string DEFAULT_SCRIPT_TAG = "Latn";
const std::string DEFAULT_FONT_PATH = "/Library/Fonts/Arial Unicode.ttf"; // OSX

class Directive
{
public:
    std::vector<std::string> lines;

    ci::fs::path fontPath;
    hb_script_t script;
    hb_direction_t direction;
    
    Directive(const std::string &text, const ci::fs::path &fontPath = DEFAULT_FONT_PATH, hb_script_t script = DEFAULT_SCRIPT, hb_direction_t direction = DEFAULT_DIRECTION);
    Directive(ci::DataSourceRef source);
    Directive(const std::exception &e);
    
protected:
    ci::fs::path getFontPath(const ci::fs::path virtualPath);
    void initText(const std::string &text);
};
