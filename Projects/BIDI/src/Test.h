#pragma once

#include "unicode/unistr.h"
#include "unicode/uscript.h"
#include "unicode/ubidi.h"

enum
{
    kBidi_LTR = 0,
    kBidi_RTL = 1,
    kBidi_Default_LTR = 2,
    kBidi_Default_RTL = 3,
    kBidi_Force_LTR = 4,
    kBidi_Force_RTL = 5,
    
    kBidi_Mask = 0x7
};

enum
{
    kDirection_LTR = 0,
    kDirection_RTL = 1,
    
    kDirection_Mask = 0x1
};

//#define SPIT_DETAILS

class Test
{
public:
    static void start(const std::string &title);
    static void spitRun(const UnicodeString &text, UBiDiDirection direction, int32_t start, int32_t end);
    
    static void bidiMapnik(const std::string &input, UBiDiLevel paraLevel = UBIDI_DEFAULT_LTR);
    static void bidiAndroid(const std::string &input, int dirFlags = kBidi_Default_LTR);
};
