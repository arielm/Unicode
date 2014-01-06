/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * IMPLEMENTING SCRIPT DETECTION USING:
 * http://source.icu-project.org/repos/icu/icu/trunk/source/extra/scrptrun
 */

#pragma once

#include "unicode/unistr.h"

#include "scrptrun.h"

class Test
{
public:
    static void run()
    {
        const std::string input = " ॆहिन्दी العربية Русский English 漢孵とひらがなとカタカナ𐐀𐐁𐐂𐐃";
        
        UnicodeString text = UnicodeString::fromUTF8(input);
        ScriptRun scriptRun(text.getBuffer(), text.length());
        
        while (scriptRun.next())
        {
            auto start = scriptRun.getScriptStart();
            auto end = scriptRun.getScriptEnd();
            auto code = scriptRun.getScriptCode();
            
            std::string tmp;
            text.tempSubString(start, end - start).toUTF8String(tmp);
            
            std::cout << "SCRIPT '" << uscript_getName(code) << "' FROM " << start << " TO " << end - 1 << std::endl;
            std::cout << tmp << std::endl << std::endl;
        }
    }
};
