#pragma once

#include "unicode/unistr.h"
#include "unicode/uscript.h"
#include "unicode/ubidi.h"

#include <string>

struct TextItem
{
    UnicodeString text;
    UScriptCode script;
    UBiDiDirection direction;

    TextItem(const UnicodeString &text, UScriptCode script = USCRIPT_LATIN, UBiDiDirection direction = UBIDI_LTR)
    :
    text(text),
    script(script),
    direction(direction)
    {}
   
    TextItem(const std::string &text, UScriptCode script = USCRIPT_LATIN, UBiDiDirection direction = UBIDI_LTR)
    :
    text(UnicodeString::fromUTF8(text)),
    script(script),
    direction(direction)
    {}
    
    std::string getUTF8Text() const
    {
        std::string output;
        return text.toUTF8String(output);
    }
};
