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
