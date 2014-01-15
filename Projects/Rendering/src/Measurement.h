/*
 * MEASURING LayoutCache PERFORMANCE
 */

#pragma once

#include "LayoutCache.h"

#include "chronotext/utils/Timing.h"

#include "cinder/Rand.h"

const size_t CACHE_SIZE = 32 * 1024; // BIG ENOUGH TO AVOID EVICTION
const uint32_t RAND_SEED = 123;
const int TIMING_COUNT = 1000;

class Measurement
{
    VirtualFont *font;
    LayoutCache cache;
    
public:
    Measurement(VirtualFont *font)
    :
    font(font),
    cache(CACHE_SIZE)
    {}
    
    void run(const std::vector<std::string> &sentences, int lineCount, int maxSentencesPerLine)
    {
        ci::Rand rnd(RAND_SEED);
        chr::Timing t1("WITH CACHE");
        
        for (int j = 0; j < TIMING_COUNT; j++)
        {
            for (int i = 0; i < lineCount; i++)
            {
                std::string line;
                int sentenceCount = rnd.nextInt(1, maxSentencesPerLine);
                
                for (int j = 0; j < sentenceCount; j++)
                {
                    line += sentences[rnd.nextInt(sentences.size())];
                    line += " ";
                }
                
                cache.getLineLayout(font, line);
            }
        }
        
        cache.clear();
        t1.stop(); // RESULT: ~0.035s ON OSX IN RELEASE-MODE (O3)
        
        // ---
        
        rnd.seed(RAND_SEED);
        chr::Timing t2("WITHOUT CACHE");

        for (int j = 0; j < TIMING_COUNT; j++)
        {
            for (int i = 0; i < lineCount; i++)
            {
                std::string line;
                int sentenceCount = rnd.nextInt(1, maxSentencesPerLine);
                
                for (int j = 0; j < sentenceCount; j++)
                {
                    line += sentences[rnd.nextInt(sentences.size())];
                    line += " ";
                }
                
                delete font->createLineLayout(line);
            }
        }
        
        t2.stop(); // RESULT: ~0.876s (~25 TIMES SLOWER)
    }
};
