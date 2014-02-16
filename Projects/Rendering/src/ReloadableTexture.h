/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "GlyphData.h"

#include "cinder/gl/gl.h"

namespace chronotext
{
    namespace zf
    {
        class ReloadableTexture
        {
        public:
            ReloadableTexture(const GlyphData &glyphData);
            ~ReloadableTexture();
            
            void unload();
            void load(const GlyphData &glyphData);
            bool isLoaded() const;
            size_t getMemoryUsage() const;
            
            void bind();
            GLuint getId() const;
            int getWidth() const;
            int getHeight() const;
            
        protected:
            GLuint textureId;
            int textureWidth;
            int textureHeight;
            bool useMipmap;
        };
    }
}

namespace chr = chronotext;
