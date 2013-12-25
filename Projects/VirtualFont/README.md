VirtualFont
-----------

**Features**

1. Displaying text in any language on OSX, iOS and Android, given the right set of fonts. From top to bottom in the image below: *Ukrainian*, *Greek*, *Arabic*, *Yiddish*, *Japanese*, *Korean*, *Thai*, *Hindi*, *Simplified Chinese*, *Czech* (text from [Pango Language Sample Table](https://github.com/alexp-sssup/pango/blob/master/pango/pango-language-sample-table.h).)
 
2. The proper set of fonts per language (including [fallback](https://github.com/arielm/Unicode/tree/master/Projects/ShapingFallback)) is defined via XML. Examples: [OSX](resources/SansSerif-osx.xml), [iOS](resources/SansSerif-ios.xml) and [Android](resources/SansSerif-android.xml).

![Screenshot](screenshot.png)

**Remarks**

1. There seems to be a [risk of rejection](http://stackoverflow.com/questions/3692812/on-ios-can-i-access-the-system-provided-fonts-ttf-file) during AppStore submission for candidate apps accessing the fonts pre-installed on iOS. It sounds odds since we're currently able to access any of the files in */System/Library/Fonts/Cache* (to be further checked...)
