/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "Directive.h"

#include "cinder/app/App.h"

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace app;

Directive::Directive(DataSourceRef source)
{
    XmlTree doc(source);
    auto rootElement = doc.getChild("text");
    
    fontPath = getFontPath(rootElement.getAttributeValue<string>("font", DEFAULT_FONT_PATH));

    auto scriptTag = rootElement.getAttributeValue<string>("script", DEFAULT_SCRIPT_TAG);
    auto script = hb_script_from_string(scriptTag.c_str(), -1);
    auto direction = hb_script_get_horizontal_direction(script);
    
    span = TextSpan(getText(rootElement.getValue()), script, direction);
}

Directive::Directive(const TextSpan &span, const fs::path &virtualFontPath)
:
span(span)
{
    fontPath = getFontPath(virtualFontPath);
}

Directive::Directive(const std::exception &e)
:
fontPath(DEFAULT_FONT_PATH)
{
    span = TextSpan("ERROR: " + string(e.what()));
}

Directive::Directive(const fs::path &fontPath, const Directive &baseDirective)
:
span(baseDirective.span),
fontPath(fontPath)
{}

string Directive::getScriptName()
{
    char buf[4];
    hb_tag_to_string(hb_script_to_iso15924_tag(span.script), buf);
    return string(buf, 4);
}

fs::path Directive::getFontPath(const fs::path virtualFontPath)
{
    fs::path assetPath = App::get()->getAssetPath(virtualFontPath);
    
    if (fs::exists(assetPath))
    {
        return assetPath;
    }
    else if (fs::exists(virtualFontPath))
    {
        return virtualFontPath;
    }
    else
    {
        throw runtime_error("FONT NOT FOUND");
    }
}

string Directive::getText(const string &text)
{
    auto rawLines = split(text, '\n');
    
    for (auto line : rawLines)
    {
        auto trimmed = boost::algorithm::trim_copy(line);
        
        if (!trimmed.empty())
        {
            return trimmed;
        }
    }
    
    return "";
}
