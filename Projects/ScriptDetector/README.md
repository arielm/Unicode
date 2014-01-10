11. ScriptDetector
==================

**Features**

1. **Script detection**, done with [ICU](http://source.icu-project.org/repos/icu/icu/trunk/source/extra/scrptrun). The scripts detected on line 1 are KATAKANA, HIRAGANA and HAN. On line 2 (from left to right): HAN, DEVANAGARI, CYRILLIC, HANGUL, LATIN and THAI. On line 3: ARABIC. On line 4: HEBREW.

2. **Language detection**, as [described by Behdad](http://www.mail-archive.com/harfbuzz@lists.freedesktop.org/msg03220.html) on the Harfbuzz mailing-list. The language detected on line 1 is Japanese. On line 2 (from left to right): Chinese, Hindi, Russian, Korean, English and Thai. On line 3: Arabic. On line 4: Hebrew.

![Screenshot](screenshot.png)

**Remarks**

1. The system allows to define some *global language hint* at the line level. For line 1, we provide a hint so that the whole content is treated as Japanese. Without it, the HAN glyphs would have been rendered in pink (i.e. with the font used to render Chinese.)  

2. We're not, however, providing any means to define *local language hints* at the line level. So for line 2, even-though the second word from the left means "Japanese", it will not be detected as "ja" because the script is HAN (in this example, the *default languages* are set to "en:zh-cn", so whenever we need to associate a language to HAN, Chinese will be picked...)

3. More details? See how [text is defined](resources/Text.xml), and [which fonts were used](resources/SansSerif.xml) to render the screenshot. There's also a [**standalone test**](src/Test.h) with insightful content. Finally, the [**LanguageHelper**](src/LanguageHelper.h) class contains a serious dose of Pango wisdom.

4. BIDI is deliberately left-aside (and properly addressed in our [BIDI](https://github.com/arielm/Unicode/tree/master/Projects/BIDI) project.) Anyway, if BIDI were taking place, we could place the Arabic and Hebrew of lines 3 and 4 (both RTL languages) on line 2. Currently, we can't even place Hebrew and Arabic properly on the same line (without BIDI, the 2 words are glued...)
