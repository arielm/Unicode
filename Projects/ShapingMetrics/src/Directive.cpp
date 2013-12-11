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
    auto rootElement = doc.getChild("span");
    
    fontPath = getFontPath(rootElement.getAttributeValue<string>("font", DEFAULT_FONT_PATH));

    auto scriptTag = rootElement.getAttributeValue<string>("script", DEFAULT_SCRIPT_TAG);
    auto script = hb_script_from_string(scriptTag.c_str(), -1);
    auto direction = hb_script_get_horizontal_direction(script);
    
    span = TextSpan(getText(rootElement.getValue()), script, direction);
}

Directive::Directive(const string &text, const fs::path &fontPath , hb_script_t script, hb_direction_t direction)
:
fontPath(fontPath)
{
    span = TextSpan(getText(text), script, direction);
}

Directive::Directive(const std::exception &e)
:
fontPath(DEFAULT_FONT_PATH)
{
    span = TextSpan("ERROR:\n" + string(e.what()));
}

Directive::Directive(const fs::path &fontPath, const Directive &baseDirective)
:
fontPath(fontPath),
span(baseDirective.span)
{}

fs::path Directive::getFontPath(const fs::path virtualPath)
{
    fs::path assetPath = App::get()->getAssetPath(virtualPath);
    
    if (fs::exists(assetPath))
    {
        return assetPath;
    }
    else if (fs::exists(virtualPath))
    {
        return virtualPath;
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
