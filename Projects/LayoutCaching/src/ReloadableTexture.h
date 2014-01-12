/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "GlyphData.h"

#include "cinder/gl/Texture.h"

class ReloadableTexture
{
public:
    ReloadableTexture(const GlyphData &glyphData);
    ~ReloadableTexture();
    
    bool isLoaded() const;
    void unload();
    void load(const GlyphData &glyphData);
    
    void bind();
    GLuint getId() const;
    int getWidth() const;
    int getHeight() const;
    
protected:
    GLuint textureId;
    int textureWidth;
    int textureHeight;
};
