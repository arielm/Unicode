13. LayoutCaching
=================

This project demonstrates effective text-layout-caching using a LRU strategy. The [cache](src/LayoutCache.h) is implemented using simple components from the *C++11 standard library*, with a [little help from boost](http://www.boost.org/doc/libs/1_53_0/libs/bimap/doc/html).

**Important**: this is the [new-chronotext-toolkit](https://github.com/arielm/new-chronotext-toolkit) version of the project. Additional requirement: [hb-icu](https://github.com/arielm/hb-icu) block.

*Action* is taking place in the [Sketch class](src/Sketch.h)...
