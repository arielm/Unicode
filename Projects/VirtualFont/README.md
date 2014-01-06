8. VirtualFont
-----------

**Features**

1. Displaying text in any language on OSX, iOS and Android, given the right set of fonts. From top to bottom in the image below: *Ukrainian*, *Greek*, *Arabic*, *Yiddish*, *Japanese*, *Korean*, *Thai*, *Hindi*, *Simplified Chinese*, *Czech* (text from [Pango Language Sample Table](https://github.com/alexp-sssup/pango/blob/master/pango/pango-language-sample-table.h).)
 
2. The proper set of fonts per language (including [fallback](https://github.com/arielm/Unicode/tree/master/Projects/ShapingFallback)) is defined via XML. Examples: [OSX](resources/SansSerif-osx.xml), [iOS](resources/SansSerif-ios.xml) and [Android](resources/SansSerif-android.xml).

![Screenshot](screenshot.png)

**Remarks**

1. There seems to be a [risk of rejection](http://stackoverflow.com/questions/3692812/on-ios-can-i-access-the-system-provided-fonts-ttf-file) during AppStore submission for candidate apps accessing the fonts pre-installed on iOS. It sounds odds since we're currently able to access any of the files in */System/Library/Fonts/Cache* (to be further checked...)

**Performance tests**

Shaping our ten lines with Harfbuzz 100 times takes (compiled with *O3*):

1. On Mac Mini: ~0.07s
2. On iPad 1: ~0.63s
3. On Android Nexus 7: ~0.30s

On the iPad 1, it corresponds to about 26 line shapings per frame (at 60 FPS), even before rendering anything. The conclusion is that shaping is a pretty slow operation. It also explains why [text-layout-caching](https://github.com/android/platform_frameworks_base/blob/677726b376402937f53ddb192dc97078b92b7c9e/core/jni/android/graphics/TextLayoutCache.cpp) is taking place in the recent Harfbuzz-flavored version of the Android OS...
