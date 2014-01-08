/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "TextItemizer.h"

#include "scrptrun.h"

using namespace std;

TextGroup TextItemizer::process(const string &input, const string &langHint, hb_direction_t overallDirection)
{
    TextGroup group(input, overallDirection);
    ScriptRun scriptRun(group.text.getBuffer(), group.text.length());
    
    while (scriptRun.next())
    {
        auto start = scriptRun.getScriptStart();
        auto end = scriptRun.getScriptEnd();
        auto code = scriptRun.getScriptCode();
        
        auto script = LanguageHelper::hbScriptFromICU(code);
        auto language = languageHelper.detectLanguage(script, langHint);
        auto direction = hb_script_get_horizontal_direction(script);
        
        group.addItem(start, end, script, language, direction);
    }
    
    return group;
}
