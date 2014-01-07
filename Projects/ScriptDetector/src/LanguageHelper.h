/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "Languages.h"

#include "cinder/Utilities.h"

class LanguageHelper
{
public:
    LanguageHelper()
    {
        Languages::initScriptMap(scriptMap);
        Languages::initSampleLanguageMap(sampleLanguageMap);
        setDefaultLanguages("en:ja");
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
    
    std::string getSampleLanguage(hb_script_t script) const
    {
        auto it = sampleLanguageMap.find(script);
        
        if (it == sampleLanguageMap.end())
        {
            it = sampleLanguageMap.find(HB_SCRIPT_INVALID);
        }
        
        return it->second;
    }
    
protected:
    std::map<std::string, std::vector<hb_script_t>> scriptMap;
    std::map<hb_script_t, std::string> sampleLanguageMap;
    std::set<std::string> defaultLanguageSet;
};
