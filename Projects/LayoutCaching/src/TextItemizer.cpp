/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * USING ICU 52.1 FOR SCRIPT-DETECTION AND FOR BIDI:
 * http://www.icu-project.org/apiref/icu4c/ubidi_8h.html
 */

/*
 * INSPIRED BY THE MAPNIK PROJECT:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/src/text/itemizer.cpp
 */

#include "TextItemizer.h"

#include "scrptrun.h"

using namespace std;

hb_script_t TextItemizer::icuScriptToHB(UScriptCode script)
{
    if (script == USCRIPT_INVALID_CODE)
    {
        return HB_SCRIPT_INVALID;
    }
    
    return hb_script_from_string(uscript_getShortName(script), -1);
}

hb_direction_t TextItemizer::icuDirectionToHB(UBiDiDirection direction)
{
    return (direction == UBIDI_RTL) ? HB_DIRECTION_RTL : HB_DIRECTION_LTR;
}

TextLine TextItemizer::process(const string &input, const string &langHint, hb_direction_t overallDirection)
{
    TextLine line(input, overallDirection);

    vector<ScriptAndLanguageRun> scriptAndLanguageRuns;
    itemizeScriptAndLanguage(line.text, langHint, scriptAndLanguageRuns);
    
    vector<DirectionRun> directionRuns;
    itemizeDirection(line.text, overallDirection, directionRuns);
    
    mergeRuns(scriptAndLanguageRuns, directionRuns, line.runs);
    return line;
}

void TextItemizer::itemizeScriptAndLanguage(const UnicodeString &text, const string &langHint, vector<ScriptAndLanguageRun> &runs)
{
    ScriptRun scriptRun(text.getBuffer(), text.length());
    
    while (scriptRun.next())
    {
        auto start = scriptRun.getScriptStart();
        auto end = scriptRun.getScriptEnd();
        auto code = scriptRun.getScriptCode();
        
        auto script = icuScriptToHB(code);
        auto language = languageHelper.detectLanguage(script, langHint);
        
        runs.emplace_back(start, end, make_pair(script, language));
    }
}

void TextItemizer::itemizeDirection(const UnicodeString &text, hb_direction_t overallDirection, vector<DirectionRun> &runs)
{
    /*
     * WE WANT TO FORCE A DIRECTION (I.E. NOT DETERMINING THE PARAGRAPH-LEVEL FROM THE TEXT)
     * SEE: http://www.icu-project.org/apiref/icu4c/ubidi_8h.html#abdfe9e113a19dd8521d3b7ac8220fe11
     */
    UBiDiLevel paraLevel = (overallDirection == HB_DIRECTION_RTL) ? 1 : 0;

    auto length = text.length();
    UErrorCode error = U_ZERO_ERROR;
    UBiDi *bidi = ubidi_openSized(length, 0, &error);
    
    ubidi_setPara(bidi, text.getBuffer(), length, paraLevel, 0, &error);
    auto direction = ubidi_getDirection(bidi);
    
    if (direction != UBIDI_MIXED)
    {
        runs.emplace_back(0, length, icuDirectionToHB(direction));
    }
    else
    {
        auto count = ubidi_countRuns(bidi, &error);
        
        for (int i = 0; i < count; ++i)
        {
            int32_t start, length;
            direction = ubidi_getVisualRun(bidi, i, &start, &length);
            runs.emplace_back(start, start + length, icuDirectionToHB(direction));
        }
    }
    
    ubidi_close(bidi);
}

void TextItemizer::mergeRuns(const vector<ScriptAndLanguageRun> &scriptAndLanguageRuns, const vector<DirectionRun> &directionRuns, vector<TextRun> &runs)
{
    for (auto &directionRun : directionRuns)
    {
        auto position = directionRun.start;
        auto end = directionRun.end;
        auto rtlInsertionPoint = runs.end();
        
        auto scriptAndLanguageIterator = findRun(scriptAndLanguageRuns, position);
        
        while (position < end)
        {
            TextRun run;
            run.start = position;
            run.end = std::min(scriptAndLanguageIterator->end, end);
            run.script = scriptAndLanguageIterator->data.first;
            run.language = scriptAndLanguageIterator->data.second;
            run.direction = directionRun.data;
            
            if (directionRun.data == HB_DIRECTION_LTR)
            {
                runs.emplace_back(run);
            }
            else
            {
                rtlInsertionPoint = runs.insert(rtlInsertionPoint, run);
            }
            
            position = run.end;
            
            if (scriptAndLanguageIterator->end == position)
            {
                ++scriptAndLanguageIterator;
            }
        }
    }
}

template <typename T>
typename T::const_iterator TextItemizer::findRun(const T &runs, int32_t position)
{
    for (auto it = runs.begin(); it != runs.end(); ++it)
    {
        if ((it->start <= position) && (it->end > position))
        {
            return it;
        }
    }
    
    return runs.end();
}
