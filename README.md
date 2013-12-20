The Unicode test suite for Cinder & C++11
=========================================

The goal is to provide building blocks for handling Unicode text rendering from the ground-up on OSX, iOS and Android (**Windows support should be trivial to add**, contributions are welcome...)  

**Topics of interest**

1. Shaping
2. BIDI reordering
3. Line-breaking
4. Font-substitution
5. Layout caching
6. OpenGL rendering

[Freetype](http://www.freetype.org) should be used behind the scenes. In addition, existing quality open-source fonts should be evaluated, starting with [those bundled with the Android platform](fonts).

**Motivations and scope**

The goal is to provide support for internationalization in interactive applications and games created with Cinder. Currently, there is no appropriate cross-platform solution due the lack of public C++ API for text-rendering on Android. 

Take in count, however, that only a subset of the features available in OS solutions like *CoreText* (or even in modern browsers) are going to be implemented.

**Progress**

So far, [Harfbuzz](https://github.com/behdad/harfbuzz) have been tested as a solution for shaping via the following projects:

1. [CinderHarfbuzz](Projects/CinderHarfbuzz)
2. [ShapingTester](Projects/ShapingTester)
3. [ShapingMetrics](Projects/ShapingMetrics)
4. [FontComparator](Projects/FontComparator)
5. [LanguageFeatures](Projects/LanguageFeatures)
6. [ShapingFallback](Projects/ShapingFallback)
7. [BIDI](Projects/BIDI)

More work remains, notably testing how Harfbuzz performs on low-end mobile devices with limited processing-power and memory.

Installation
------------

The instructions are for OSX.  

1. Download [Cinder 0.8.5 Vanilla](http://libcinder.org/releases/cinder_0.8.5_mac.zip)
2. Unzip and rename the "cinder_0.8.5_mac" folder to "Cinder"
3. Place a copy of the [Freetype repository](https://github.com/arielm/Freetype) in `Cinder/blocks`
4. Place a copy of this repository in `Cinder/samples`

The folder hierarchy should look like:  
`Cinder`  
`|--blocks`  
`|..|--Freetype`  
`|--samples`  
`|..|--Unicode`  

**Running on OSX and iOS**

1. Enter the relevant folder in `Projects`
2. Open the relevant XCode project (under `osx` or `ios`)

Tested with OSX 10.7.5 and XCode 4.6.2.  
Tested with OSX 10.5.8 and XCode 5.0.2.  

**Running on Android**

Prerequisites:

1. [Safetydank's fork of Cinder](https://github.com/safetydank/Cinder)
2. [NDK r8e](http://dl.google.com/android/ndk/android-ndk-r8e-darwin-x86_64.tar.bz2)
3. [ADT 22.0.5](http://dl.google.com/android/adt/adt-bundle-mac-x86_64-20130729.zip)

Then:

1. Enter the relevant folder in `Projects`
2. Enter the `android` folder
3. Type `. ./setup-android` in Terminal and follow the instructions
