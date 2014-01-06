2. ShapingTester
-------------

**Instructions**

1. Drag-and-drop some directive file from the project's [assets/directives](assets/directives) folder.
2. Or a font (.ttf or .otf) from the repository's [assets/fonts](https://github.com/arielm/Unicode/tree/master/fonts) folder or from anywhere else on your system.

![Screenshot](screenshot.png)

Installation instructions and more at https://github.com/arielm/Unicode  

Executable available for OSX: [v1.0.0](https://github.com/arielm/Unicode/releases/download/v1.0.0/Unicode-1.0.0.zip)

**Creating symbolic links to access your system's fonts via the Finder on OSX 10.7 and 10.8**

`mkdir ~/Documents/Fonts`  
`ln -s /Library/Fonts/* ~/Documents/Fonts`  
`ln -s /System/Library/Fonts/* ~/Documents/Fonts`  

**Remarks**

1. Rendering is not optimized.
2. It became clear [later on](https://github.com/arielm/Unicode/tree/master/Projects/LanguageFeatures) that it is necessary to specify **both a script and a language**.
3. Clarification with Japanese: either *HB_SCRIPT_HAN*, *HB_SCRIPT_KATAKANA* or *HB_SCRIPT_HIRAGANA* can be used, as long as "ja" is defined as language, and a font like *MTLmr3m.ttf* is used (with a font like *DroidSansFallback.ttf*, some of the Kanji glyph are shaped in a Chinese fashion...)
