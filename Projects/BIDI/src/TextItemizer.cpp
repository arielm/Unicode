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

    auto text = group.text.getBuffer();
    auto length = group.text.length();
    
    /*
     * SCRIPT AND LANGUAGE ITEMIZATION
     */
    
    std::vector<ScriptAndLanguageRun> scriptAndLanguageRuns;
    ScriptRun scriptRun(text, length);
    
    while (scriptRun.next())
    {
        auto start = scriptRun.getScriptStart();
        auto end = scriptRun.getScriptEnd();
        auto code = scriptRun.getScriptCode();
        
        auto script = icuScriptToHB(code);
        auto language = languageHelper.detectLanguage(script, langHint);
        
        scriptAndLanguageRuns.emplace_back(start, end, make_pair(script, language));
    }
    
    /*
     * BIDI ITEMIZATION
     */
    
    std::vector<DirectionRun> directionRuns;
    
    UErrorCode error = U_ZERO_ERROR;
    UBiDi *bidi = ubidi_openSized(length, 0, &error);
    
    ubidi_setPara(bidi, text, length, hbDirectionToUBIDILevel(overallDirection), 0, &error);
    auto direction = ubidi_getDirection(bidi);
    
    if (direction != UBIDI_MIXED)
    {
        directionRuns.emplace_back(0, length, uciDirectionToHB(direction));
    }
    else
    {
        auto count = ubidi_countRuns(bidi, &error);
        
        for (int i = 0; i < count; ++i)
        {
            int32_t start, length;
            direction = ubidi_getVisualRun(bidi, i, &start, &length);
            directionRuns.emplace_back(start, start + length, uciDirectionToHB(direction));
        }
    }
    
    ubidi_close(bidi);
    
    /*
     * MIX
     */
    
    auto &output = group.items;
    
    for (auto &directionRun : directionRuns)
    {
        auto position = directionRun.start;
        auto end = directionRun.end;
        auto rtlInsertionPoint = output.end();
        
        auto scriptAndLanguageIt = findRun(scriptAndLanguageRuns, position);
        
        while (position < end)
        {
//            assert(scriptAndLanguageIt != scriptAndLanguageRuns.end());
            
            TextItem item;
            item.start = position;
            position = std::min(scriptAndLanguageIt->end, end);
            item.end = position;
            item.script = scriptAndLanguageIt->data.first;
            item.lang = scriptAndLanguageIt->data.second;
            item.direction = directionRun.data;
            
            if (directionRun.data == HB_DIRECTION_LTR)
            {
                output.push_back(item);
            }
            else
            {
                rtlInsertionPoint = output.insert(rtlInsertionPoint, item);
            }
            
            if (scriptAndLanguageIt->end == position)
            {
                ++scriptAndLanguageIt;
            }
        }
    }
    
    return group;
}

hb_script_t TextItemizer::icuScriptToHB(UScriptCode script)
{
    if (script == USCRIPT_INVALID_CODE)
    {
        return HB_SCRIPT_INVALID;
    }
    
    return hb_script_from_string(uscript_getShortName(script), -1);
}

hb_direction_t TextItemizer::uciDirectionToHB(UBiDiDirection direction)
{
    return (direction == UBIDI_RTL) ? HB_DIRECTION_RTL : HB_DIRECTION_LTR;
}

/*
 * WE WANT TO FORCE A DIRECTION (I.E. NOT DETERMINING THE PARAGRAPH-LEVEL FROM THE TEXT)
 * SEE: http://www.icu-project.org/apiref/icu4c/ubidi_8h.html#abdfe9e113a19dd8521d3b7ac8220fe11
 */
UBiDiLevel TextItemizer::hbDirectionToUBIDILevel(hb_direction_t direction)
{
    return (direction == HB_DIRECTION_RTL) ? 1 : 0;
}

template <typename T>
typename T::const_iterator TextItemizer::findRun(const T &list, unsigned position)
{
    auto it = list.begin();
    
    for (; it != list.end(); ++it)
    {
        if ((it->start <= position) && (it->end > position))
        {
            return it;
        }
    }
    
    return it;
}
