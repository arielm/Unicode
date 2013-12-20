#include "TextGroup.h"

using namespace std;

TextGroup::TextGroup(const string &input, hb_script_t script, const string &lang, hb_direction_t overallDirection)
:
script(script),
lang(lang),
overallDirection(overallDirection)
{
    if (hb_script_get_horizontal_direction(script) == HB_DIRECTION_LTR)
    {
        addRun(input, HB_DIRECTION_LTR);
    }
    else
    {
        auto text = UnicodeString::fromUTF8(input);
        auto length = text.length();
        
        UErrorCode error = U_ZERO_ERROR;
        UBiDi *bidi = ubidi_openSized(length, 0, &error);
        
        ubidi_setPara(bidi, text.getBuffer(), length, hbDirectionToUCI(overallDirection), 0, &error);
        auto direction = ubidi_getDirection(bidi);
        
        if (direction != UBIDI_MIXED)
        {
            addRun(input, uciDirectionToHB(direction));
        }
        else
        {
            auto count = ubidi_countRuns(bidi, &error);
            
            for (int i = 0; i < count; ++i)
            {
                int32_t start, length;
                direction = ubidi_getVisualRun(bidi, i, &start, &length);
                addRun(text, direction, start, start + length);
            }
        }
        
        ubidi_close(bidi);
    }
}

hb_direction_t TextGroup::uciDirectionToHB(UBiDiDirection direction)
{
    return (direction == UBIDI_RTL) ? HB_DIRECTION_RTL : HB_DIRECTION_LTR;
}

UBiDiDirection TextGroup::hbDirectionToUCI(hb_direction_t direction)
{
    return (direction == HB_DIRECTION_RTL) ? UBIDI_RTL : UBIDI_LTR;
}

void TextGroup::addRun(const string &text, hb_direction_t direction)
{
    runs.emplace_back(text, script, lang, direction);
}

void TextGroup::addRun(const UnicodeString &input, UBiDiDirection direction, int32_t start, int32_t end)
{
    string text;
    input.tempSubString(start, end - start).toUTF8String(text);
    
    runs.emplace_back(text, script, lang, uciDirectionToHB(direction));
}
