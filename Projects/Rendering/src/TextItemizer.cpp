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

TextItemizer::TextItemizer(LangHelper &langHelper)
:
langHelper(langHelper)
{}

TextLine TextItemizer::processLine(const string &input, const string &langHint, hb_direction_t overallDirection)
{
    TextLine line(input, langHint, overallDirection);

    vector<ScriptAndLanguageItem> scriptAndLanguageItems;
    itemizeScriptAndLanguage(line.text, langHint, scriptAndLanguageItems);
    
    vector<DirectionItem> directionItems;
    itemizeDirection(line.text, overallDirection, directionItems);
    
    mergeItems(scriptAndLanguageItems, directionItems, line.runs);
    
    if (!line.runs.empty())
    {
        if (langHint.empty())
        {
            line.langHint = line.runs[0].language;
        }
        
        if (overallDirection == HB_DIRECTION_INVALID)
        {
            line.overallDirection = line.runs[0].direction;
        }
    }
    
    return line;
}

void TextItemizer::itemizeScriptAndLanguage(const UnicodeString &text, const string &langHint, vector<ScriptAndLanguageItem> &items)
{
    ScriptRun scriptRun(text.getBuffer(), text.length());
    
    while (scriptRun.next())
    {
        auto start = scriptRun.getScriptStart();
        auto end = scriptRun.getScriptEnd();
        auto code = scriptRun.getScriptCode();
        
        auto script = icuScriptToHB(code);
        auto language = langHelper.detectLanguage(script, langHint);
        
        items.emplace_back(start, end, make_pair(script, language));
    }
}

void TextItemizer::itemizeDirection(const UnicodeString &text, hb_direction_t overallDirection, vector<DirectionItem> &items)
{
    /*
     * IF overallDirection IS UNDEFINED: THE PARAGRAPH-LEVEL WILL BE DETERMINED FROM THE TEXT
     *
     * SEE: http://www.icu-project.org/apiref/icu4c/ubidi_8h.html#abdfe9e113a19dd8521d3b7ac8220fe11
     */
    UBiDiLevel paraLevel = (overallDirection == HB_DIRECTION_INVALID) ? UBIDI_DEFAULT_LTR : ((overallDirection == HB_DIRECTION_RTL) ? 1 : 0);

    auto length = text.length();
    UErrorCode error = U_ZERO_ERROR;
    UBiDi *bidi = ubidi_openSized(length, 0, &error);
    
    ubidi_setPara(bidi, text.getBuffer(), length, paraLevel, 0, &error);
    auto direction = ubidi_getDirection(bidi);
    
    if (direction != UBIDI_MIXED)
    {
        items.emplace_back(0, length, icuDirectionToHB(direction));
    }
    else
    {
        auto count = ubidi_countRuns(bidi, &error);
        
        for (int i = 0; i < count; ++i)
        {
            int32_t start, length;
            direction = ubidi_getVisualRun(bidi, i, &start, &length);
            items.emplace_back(start, start + length, icuDirectionToHB(direction));
        }
    }
    
    ubidi_close(bidi);
}

void TextItemizer::mergeItems(const vector<ScriptAndLanguageItem> &scriptAndLanguageItems, const vector<DirectionItem> &directionItems, vector<TextRun> &runs)
{
    for (auto &directionItem : directionItems)
    {
        auto position = directionItem.start;
        auto end = directionItem.end;
        auto rtlInsertionPoint = runs.end();
        
        auto scriptAndLanguageIterator = findItem(scriptAndLanguageItems, position);
        
        while (position < end)
        {
            TextRun run;
            run.start = position;
            run.end = std::min(scriptAndLanguageIterator->end, end);
            run.script = scriptAndLanguageIterator->data.first;
            run.language = scriptAndLanguageIterator->data.second;
            run.direction = directionItem.data;
            
            if (directionItem.data == HB_DIRECTION_LTR)
            {
                runs.push_back(run);
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
typename T::const_iterator TextItemizer::findItem(const T &items, int32_t position)
{
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if ((it->start <= position) && (it->end > position))
        {
            return it;
        }
    }
    
    return items.end();
}
