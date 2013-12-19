#pragma once

#include "TextSpan.h"

#include "unicode/unistr.h"
#include "unicode/uscript.h"
#include "unicode/ubidi.h"

#include <vector>

class BidiProcessor
{
public:
    BidiProcessor(const std::string &input, hb_script_t script = HB_SCRIPT_LATIN, const std::string &lang = "", hb_direction_t direction = HB_DIRECTION_LTR);
    
    std::vector<TextSpan> getRuns() const;
    
    static hb_direction_t uciDirectionToHB(UBiDiDirection direction);
    static UBiDiDirection hbDirectionToUCI(hb_direction_t direction);

protected:
    hb_script_t script;
    std::string lang;
    
    std::vector<TextSpan> runs;

    void addRun(const UnicodeString &text, UBiDiDirection direction, int32_t start, int32_t end);
};
