/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "LanguageHelper.h"
#include "TextGroup.h"

#include "unicode/unistr.h"
#include "unicode/uscript.h"
#include "unicode/ubidi.h"

class TextItemizer
{
public:
    static hb_script_t icuScriptToHB(UScriptCode script);
    static hb_direction_t icuDirectionToHB(UBiDiDirection direction);

    TextGroup process(const std::string &input, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_LTR);
    
protected:
    LanguageHelper languageHelper;
    
    template<typename T> struct Run
    {
        int32_t start;
        int32_t end;
        T data;

        Run(int32_t start, int32_t end, T data)
        :
        start(start),
        end(end),
        data(data)
        {}
    };

    typedef Run<std::pair<hb_script_t, std::string>> ScriptAndLanguageRun;
    typedef Run<hb_direction_t> DirectionRun;

    void itemizeScriptAndLanguage(const UnicodeString &text, const std::string &langHint, std::vector<ScriptAndLanguageRun> &runs);
    void itemizeDirection(const UnicodeString &text, hb_direction_t overallDirection, std::vector<DirectionRun> &runs);
    void mergeRuns(const std::vector<ScriptAndLanguageRun> &scriptAndLanguageRuns, const std::vector<DirectionRun> &directionRuns, std::vector<TextItem> &items);
    
    template<typename T> typename T::const_iterator findRun(const T &runs, int32_t position);
};
