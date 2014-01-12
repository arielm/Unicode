/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * REFERENCE: https://github.com/arielm/Unicode/tree/master/Projects/BIDI
 */

/*
 * USING ICU 52.1 FOR SCRIPT-DETECTION AND FOR BIDI:
 * http://www.icu-project.org/apiref/icu4c/ubidi_8h.html
 */

/*
 * INSPIRED BY THE MAPNIK PROJECT:
 * https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/src/text/itemizer.cpp
 *
 *
 * SOME DIFFERENCES:
 *
 * 1) WE'RE NOT HANDLING STYLE
 *    BUT THE Run TEMPLATE TOGETHER WITH THE mergeRuns() AND findRun()
 *    FUNCTIONS ARE PREPARING THE TERRAIN FOR FUTURE IMPLEMENTATION
 *
 * 2) WE'RE DOING "LANGUAGE DETECTION" IN ADDITION TO "SCRIPT DETECTION"
 *    NECESSARY IN ORDER TO HANDLE PROPERLY LANGUAGES SHARING THE SAME SCRIPT
 *
 * 3) WE'RE NOT DETERMINING THE PARAGRAPH-LEVEL FROM THE TEXT
 *    NECESSARY IN ORDER TO PROPERLY HANDLE CERTAIN COMBINATIONS OF LTR AND RTL
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
    void mergeRuns(const std::vector<ScriptAndLanguageRun> &scriptAndLanguageRuns, const std::vector<DirectionRun> &directionRuns, std::vector<TextRun> &runs);
    
    template<typename T> typename T::const_iterator findRun(const T &runs, int32_t position);
};
