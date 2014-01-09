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
    TextGroup process(const std::string &input, const std::string &langHint = "", hb_direction_t overallDirection = HB_DIRECTION_LTR);
    
    static hb_script_t icuScriptToHB(UScriptCode script);
    static hb_direction_t uciDirectionToHB(UBiDiDirection direction);
    static UBiDiLevel hbDirectionToUBIDILevel(hb_direction_t direction);
    
protected:
    LanguageHelper languageHelper;
    
    template<typename T> struct Run
    {
        unsigned start;
        unsigned end;
        T data;

        Run(unsigned start, unsigned end, T data)
        :
        start(start),
        end(end),
        data(data)
        {}
    };

    typedef Run<std::pair<hb_script_t, std::string>> ScriptAndLanguageRun;
    typedef Run<hb_direction_t> DirectionRun;
    
    template<typename T> typename T::const_iterator findRun(const T &list, unsigned position);
};
