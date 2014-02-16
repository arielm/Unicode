/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

/*
 * INCLUDES DATA FROM PANGO:
 * https://developer.gnome.org/pango/stable/pango-Scripts-and-Languages.html
 */

#pragma once

#include "hb.h"

#include <vector>
#include <map>
#include <set>
#include <string>

namespace chronotext
{
    namespace zf
    {
        class LangHelper
        {
        public:
            LangHelper();
            
            /*
             * EXPECTS A LIST LANGUAGES SEPARATED BY COLONS
             */
            void setDefaultLanguages(const std::string &languages);
            
            /*
             * DETERMINES THE SCRIPTS USED TO WRITE @lang
             *
             * QUOTING PANGO:
             * Most languages use only one script for writing, but there are
             * some that use two (Latin and Cyrillic for example), and a few
             * use three (Japanese for example).
             */
            const std::vector<hb_script_t>& getScriptsForLang(const std::string &lang) const;
            
            /*
             * DETERMINES IF @script MAY BE USED TO WRITE @lang
             */
            bool includesScript(const std::string &lang, hb_script_t script) const;
            
            /*
             * RETURNS THE RESOLVED LANGUAGE IF @script MAY BE USED TO WRITE ONE OF THE "DEFAULT LANGUAGES"
             */
            std::string getDefaultLanguage(hb_script_t script) const;
            
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
            std::string getSampleLanguage(hb_script_t script) const;
            
            /*
             * TRYING TO DETECT A LANGUAGE FOR @script BY ASKING 3 QUESTIONS:
             *
             * 1. CAN @script BE USED TO WRITE @langHint?
             * 2. CAN @script BE USED TO WRITE ONE OF THE "DEFAULT LANGUAGES"?
             * 3. IS THERE A PREDOMINANT LANGUAGE THAT IS LIKELY FOR @script?
             */
            std::string detectLanguage(hb_script_t script, const std::string &langHint = "") const;
            
        protected:
            std::map<std::string, std::vector<hb_script_t>> scriptMap;
            std::map<hb_script_t, std::string> sampleLanguageMap;
            std::set<std::string> defaultLanguageSet;
        };
    }
}

namespace chr = chronotext;
