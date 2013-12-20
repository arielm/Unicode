BIDI
----

Using text examples from [what you need to know about the bidi algorithm and inline markup](http://people.w3.org/rishida/scripts/bidi). RTL text runs are rendered in yellow. 

**Features**

1. Simple BIDI reordering: takes some text (single script and language) with *overall direction* and splits it into runs (reordering will be effectively performed only for RTL scripts.)

2. Line layout: takes a list of text-runs ands shapes them with Harfbuzz (including *font fallback*) into ready-to-be-rendered clusters.

![Screenshot](screenshot.png)

**Remarks**

1. [ICU 52.1](http://www.icu-project.org/apiref/icu4c/ubidi_8h.html) is used. It has been properly tested on OSX, iOS and Android.

2. See [Test.h](src/Test.h) for more details about the *starting points*.

3. Itemization (style and script) is not handled. Implementation [demonstrated in Mapnik](https://github.com/mapnik/mapnik/blob/64d5153aeaeb1c9e736bfead297dfea39b066d2c/src/text/itemizer.cpp)...

4. [This](https://github.com/arielm/Unicode/blob/b8223e13af4480ea1136d20165690c95b875c9ee/Projects/BIDI/src/TextGroup.cpp#L17-42) is appropriate only for one-liners. [That](http://www.icu-project.org/apiref/icu4c/ubidi_8h.html#details) is taking whole paragraphs into consideration...
