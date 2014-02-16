/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "ReloadableTexture.h"

#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    namespace zf
    {
        ReloadableTexture::ReloadableTexture(const GlyphData &glyphData)
        :
        textureId(0)
        {
            load(glyphData);
        }
        
        ReloadableTexture::~ReloadableTexture()
        {
            unload();
        }
        
        void ReloadableTexture::unload()
        {
            if (textureId)
            {
                glDeleteTextures(1, &textureId);
                textureId = 0;
            }
        }
        
        void ReloadableTexture::load(const GlyphData &glyphData)
        {
            unload();
            
            useMipmap = glyphData.useMipmap;
            int width = glyphData.width;
            int height = glyphData.height;
            int padding = glyphData.padding;
            auto buffer = glyphData.getBuffer();
            
            textureWidth = nextPowerOfTwo(width + padding * 2);
            textureHeight = nextPowerOfTwo(height + padding * 2);
            auto textureData = new unsigned char[textureWidth * textureHeight](); // ZERO-FILLED
            
            for (int iy = 0; iy < height; iy++)
            {
                for (int ix = 0; ix < width; ix++)
                {
                    textureData[(iy + padding) * textureWidth + (ix + padding)] = buffer[iy * width + ix];
                }
            }
            
            // ---
            
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            
            if (useMipmap)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            if (useMipmap)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
                glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
            }
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, textureWidth, textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, textureData);
            
            if (useMipmap)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
            }
            
            glBindTexture(GL_TEXTURE_2D, 0);
            delete[] textureData;
        }
        
        bool ReloadableTexture::isLoaded() const
        {
            return (textureId != 0);
        }
        
        size_t ReloadableTexture::getMemoryUsage() const
        {
            if (textureId)
            {
                if (useMipmap)
                {
                    return size_t(textureWidth * textureHeight * 1.333f);
                }
                else
                {
                    return textureWidth * textureHeight;
                }
            }
            else
            {
                return 0;
            }
        }
        
        void ReloadableTexture::bind()
        {
            glBindTexture(GL_TEXTURE_2D, textureId);
        }
        
        GLuint ReloadableTexture::getId() const
        {
            return textureId;
        }
        
        int ReloadableTexture::getWidth() const
        {
            return textureWidth;
        }
        
        int ReloadableTexture::getHeight() const
        {
            return textureHeight;
        }
    }
}
