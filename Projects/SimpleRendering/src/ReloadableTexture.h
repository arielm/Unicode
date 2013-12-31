/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/gl/Texture.h"

class ReloadableTexture
{
    ci::gl::Texture *tmp;
    
public:
    ReloadableTexture(unsigned char *data, int width, int height, bool useMipmap, int padding);
    ~ReloadableTexture();
    
    void bind();
    
    GLint getWidth() const;
    GLint getHeight() const;
};
