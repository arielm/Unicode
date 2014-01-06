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
const std::string DEFAULT_FONT_PATH = "/Library/Fonts/Arial Unicode.ttf"; // WARNING: OSX ONLY, WITH "BAD DIACRITICS" FOR LANGUAGES LIKE HEBREW

class Directive
{
public:
    std::vector<std::string> lines;
    hb_script_t script;
    hb_direction_t direction;
    ci::fs::path fontPath;
    
    Directive(const std::string &text, hb_script_t script = DEFAULT_SCRIPT, hb_direction_t direction = DEFAULT_DIRECTION, const ci::fs::path &virtualFontPath = DEFAULT_FONT_PATH);
    Directive(ci::DataSourceRef source);
    Directive(const std::exception &e);
    Directive(const ci::fs::path &fontPath, const Directive &baseDirective);
    
    std::string getScriptName();
    
protected:
    ci::fs::path getFontPath(const ci::fs::path virtualPath);
    void initText(const std::string &text);
};
