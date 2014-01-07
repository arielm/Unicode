/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "LanguageHelper.h"

#include "unicode/unistr.h"
#include "scrptrun.h"

class Test
{
    LanguageHelper languageHelper; // THE "DEFAULT LANGUAGES" ARE SET TO "en:zh-cn"
    
public:
    void run()
    {
        /*
         * THIS JAPANESE SENTENCE CONTAINS 3 SCRIPTS: HAN, KATAKANA AND HIRAGANA
         * WITHOUT THE "ja" HINT, THE HAN SCRIPTS WOULD HAVE BEEN DETECTED AS "zh-cn"
         */
        detectScriptsAndLanguages("ユニコードは、すべての文字に固有の番号を付与します", "ja");
        
        /*
         * UNLESS WE CHANGE THE "DEFAULT LANGUAGES"...
         */
        languageHelper.setDefaultLanguages("en:ja");
        detectScriptsAndLanguages("ユニコードは、すべての文字に固有の番号を付与します"); // NO NEED FOR A HINT ANYMORE!
        
        /*
         * BY CHANGING THE "DEFAULT LANGUAGES" OR BY
         * PROVIDING A HINT, WE COULD, IN THE FOLLOWING, TURN:
         * - ARABIC INTO PERSIAN
         * - RUSSIAN INTO UKRANIAN
         * - PART OF THE JAPANESE INTO CHINESE (OR WAS IT MEANT TO BE CHINESE IN THE FIRST PLACE?)
         */
        detectScriptsAndLanguages(" ॆहिन्दी العربية Русский English 漢孵とひらがなとカタカナ𐐀𐐁𐐂𐐃");
        
        detectScriptsAndLanguages("The title says \"W3C פעילות הבינאום,\u200f\" in Hebrew.");
        detectScriptsAndLanguages("The title is \"مفتاح معايير الويب!\u200f\" in Arabic.");
    }
    
    void detectScriptsAndLanguages(const std::string &input, const std::string &langHint = "")
    {
        auto text = UnicodeString::fromUTF8(input);
        ScriptRun scriptRun(text.getBuffer(), text.length());
        
        while (scriptRun.next())
        {
            auto start = scriptRun.getScriptStart();
            auto end = scriptRun.getScriptEnd();
            auto code = scriptRun.getScriptCode();
            
            auto language = languageHelper.detectLanguage(code, langHint);
            
            std::string tmp;
            text.tempSubString(start, end - start).toUTF8String(tmp);
            
            std::cout << "{'" << uscript_getName(code) << "','" << language << "'} FROM " << start << " TO " << end - 1 << std::endl;
            std::cout << tmp << std::endl << std::endl;
        }
        
        std::cout << "----------" << std::endl << std::endl;
    }
};
