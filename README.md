The Unicode test suite for Cinder & C++11
=======

The goal is to provide building blocks for handling Unicode text rendering from the ground-up on OSX, iOS and Android (Windows support should be trivial to add, contributions are welcome...)  

**Topics of interest**

1. Shaping
2. BIDI
3. Line-breaking
4. Font-substitution
5. Layout (post BIDI shaped glyph positions) caching
6. OpenGL rendering

[Freetype](http://www.freetype.org) should be used behind the scenes. In addition, existing quality open-source fonts should be evaluated, starting with [those bundled with the Android platform](fonts).

**Progress**

So far, [Harfbuzz](https://github.com/behdad/harfbuzz) have been tested as a solution for shaping via the following projects:

1. [CinderHarfbuzz](Projects/CinderHarfbuzz)
2. [ShapingTester](Projects/ShapingTester)

More work remains, notably testing the performance on low-end mobile devices with limited processing-power and memory.

**Remarks**

Rendering in the current projects is not optimized at all. Eventually, texture-atlases and vertex-buffers will be supported.
