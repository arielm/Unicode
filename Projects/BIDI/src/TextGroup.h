#pragma once

#include "TextSpan.h"

#include "unicode/unistr.h"
#include "unicode/uscript.h" // ???
#include "unicode/ubidi.h"

#include <vector>

class TextGroup
{
public:
    hb_direction_t overallDirection;
    std::vector<TextSpan> runs;

    TextGroup(const std::string &input, hb_script_t script = HB_SCRIPT_LATIN, const std::string &lang = "", hb_direction_t overallDirection = HB_DIRECTION_LTR);
    
    static hb_direction_t uciDirectionToHB(UBiDiDirection direction);
    static UBiDiDirection hbDirectionToUCI(hb_direction_t direction);

protected:
    hb_script_t script;
    std::string lang;

    void addRun(const std::string &text, hb_direction_t direction);
    void addRun(const UnicodeString &input, UBiDiDirection direction, int32_t start, int32_t end);
};
