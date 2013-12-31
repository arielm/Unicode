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
    
    void load(const GlyphData &glyphData);
    void bind();
    
    GLint getWidth() const;
    GLint getHeight() const;
    
protected:
    ci::gl::Texture *tmp;
};
