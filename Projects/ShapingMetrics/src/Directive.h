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

const std::string DEFAULT_SCRIPT_TAG = "Latn";
const std::string DEFAULT_FONT_PATH = "/Library/Fonts/Arial Unicode.ttf"; // OSX

class Directive
{
public:
    TextSpan span;
    ci::fs::path fontPath;
    
    Directive(const TextSpan &span, const ci::fs::path &virtualFontPath = DEFAULT_FONT_PATH);
    Directive(ci::DataSourceRef source);
    Directive(const std::exception &e);
    Directive(const ci::fs::path &fontPath, const Directive &baseDirective);
    
protected:
    ci::fs::path getFontPath(const ci::fs::path virtualFontPath);
    std::string getText(const std::string &text);
};
