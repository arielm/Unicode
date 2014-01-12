9. SimpleLayoutCaching
================

The [VirtualFont](https://github.com/arielm/Unicode/tree/master/Projects/VirtualFont) project demonstrated that shaping can be a slow operation. We've therefore implemented a simple text-layout-caching system inspired by the [solution implemented on the Android platform](https://github.com/android/platform_frameworks_base/blob/677726b376402937f53ddb192dc97078b92b7c9e/core/jni/android/graphics/TextLayoutCache.cpp).

**TODO**

1. Currently, the font as well as the text language, script and direction are used when computing the *cache-key*. We should also take in count additional styling properties like font-size or color.  

2. Some *LRU strategy* should be implemented in order to limit the size of the cache.
