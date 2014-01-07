/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "Languages.h"

#include "unicode/uscript.h"

#include "cinder/Utilities.h"

class LanguageHelper
{
public:
    LanguageHelper()
    {
        Languages::initScriptMap(scriptMap);
        Languages::initSampleLanguageMap(sampleLanguageMap);
        setDefaultLanguages("en:zh-cn");
    }
    
    void setDefaultLanguages(const std::string &languages)
    {
        defaultLanguageSet.clear();
        
        for (auto &lang : ci::split(languages, ":"))
        {
            defaultLanguageSet.insert(lang);
        }
    }

    /*
     * DETERMINES THE SCRIPTS USED TO WRITE @lang
     *
     * QUOTING PANGO:
     * Most languages use only one script for writing, but there are
     * some that use two (Latin and Cyrillic for example), and a few
     * use three (Japanese for example).
     */
    const std::vector<hb_script_t>& scriptsForLang(const std::string &lang) const
    {
        auto it = scriptMap.find(lang);
        
        if (it == scriptMap.end())
        {
            it = scriptMap.find("");
        }
        
        return it->second;
    }
    
    /*
     * DETERMINES IF @script IS ONE OF THE SCRIPTS USED TO WRITE @lang
     */
    bool includesScript(const std::string &lang, hb_script_t script) const
    {
        for (auto &value : scriptsForLang(lang))
        {
            if (value == script)
            {
                return true;
            }
        }
        
        return false;
    }
    
    /*
     * RETURNS THE RESOLVED LANGUAGE IF @script MAY BE USED TO WRITE ONE OF THE "DEFAULT LANGUAGES"
     */
    std::string getDefaultLanguage(hb_script_t script) const
    {
        for (auto &lang : defaultLanguageSet)
        {
            for (auto &value : scriptsForLang(lang))
            {
                if (value == script)
                {
                    return lang;
                }
            }
        }
        
        return "";
    }
    
    /*
     * QUOTING PANGO:
     *
     * Given a script, finds a language tag that is reasonably
     * representative of that script. This will usually be the
     * most widely spoken or used language written in that script:
     * for instance, the sample language for %HB_SCRIPT_CYRILLIC
     * is <literal>ru</literal> (Russian), the sample language
     * for %HB_SCRIPT_ARABIC is <literal>ar</literal>.
     *
     * For some
     * scripts, no sample language will be returned because there
     * is no language that is sufficiently representative. The best
     * example of this is %HB_SCRIPT_HAN, where various different
     * variants of written Chinese, Japanese, and Korean all use
     * significantly different sets of Han characters and forms
     * of shared characters. No sample language can be provided
     * for many historical scripts as well.
     */
    std::string getSampleLanguage(hb_script_t script) const
    {
        auto it = sampleLanguageMap.find(script);
        
        if (it == sampleLanguageMap.end())
        {
            it = sampleLanguageMap.find(HB_SCRIPT_INVALID);
        }
        
        return it->second;
    }
    
    static hb_script_t hbScriptFromICU(UScriptCode script)
    {
        if (script == USCRIPT_INVALID_CODE)
        {
            return HB_SCRIPT_INVALID;
        }
        
        return hb_script_from_string(uscript_getShortName(script), -1);
    }
    
    inline std::string detectLanguage(UScriptCode script, const std::string itemLanguage = "") const
    {
        return detectLanguage(hbScriptFromICU(script), itemLanguage);
    }
    
    std::string detectLanguage(hb_script_t script, const std::string itemLanguage = "") const
    {
        /*
         * 1. CAN @script BE USED TO WRITE @itemLanguage?
         */
        if (!itemLanguage.empty() && includesScript(itemLanguage, script))
        {
            return itemLanguage;
        }
        
        /*
         * 2. CAN @script BE USED TO WRITE ONE OF THE "DEFAULT LANGUAGES"?
         */
        
        auto defaultLanguage = getDefaultLanguage(script);
        
        if (!defaultLanguage.empty())
        {
            return defaultLanguage;
        }

        /*
         * 3. IS THERE A PREDOMINANT LANGUAGE THAT IS LIKELY FOR @script?
         */
        return getSampleLanguage(script);
    }
    
protected:
    std::map<std::string, std::vector<hb_script_t>> scriptMap;
    std::map<hb_script_t, std::string> sampleLanguageMap;
    std::set<std::string> defaultLanguageSet;
};
