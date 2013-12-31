/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "ReloadableTexture.h"

using namespace std;
using namespace ci;

static int nextPowerOfTwo(int x)
{
    int result = 1;
    
    while (result < x)
    {
        result <<= 1;
    }
    
    return result;
}

ReloadableTexture::ReloadableTexture(const GlyphData &glyphData)
:
tmp(NULL)
{
    load(glyphData);
}

ReloadableTexture::~ReloadableTexture()
{
    delete tmp;
}

void ReloadableTexture::load(const GlyphData &glyphData)
{
    if (tmp)
    {
        delete tmp;
    }
    
    int width = glyphData.width;
    int height = glyphData.height;
    int padding = glyphData.padding;
    auto buffer = glyphData.getBuffer();
    
    int textureWidth = nextPowerOfTwo(width + padding * 2);
    int textureHeight = nextPowerOfTwo(height + padding * 2);
    auto textureData = new unsigned char[textureWidth * textureHeight](); // ZERO-FILLED
    
    for (int iy = 0; iy < height; iy++)
    {
        for (int ix = 0; ix < width; ix++)
        {
            textureData[(iy + padding) * textureWidth + (ix + padding)] = buffer[iy * width + ix];
        }
    }
    
    gl::Texture::Format format;
    format.setInternalFormat(GL_ALPHA);
    
    if (glyphData.useMipmap)
    {
        format.enableMipmapping(true);
        format.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    }
    
    tmp = new gl::Texture(textureData, GL_ALPHA, textureWidth, textureHeight, format);
    delete[] textureData;
}

void ReloadableTexture::bind()
{
    tmp->bind();
}

GLint ReloadableTexture::getWidth() const
{
    return tmp->getWidth();
}

GLint ReloadableTexture::getHeight() const
{
    return tmp->getHeight();
}
