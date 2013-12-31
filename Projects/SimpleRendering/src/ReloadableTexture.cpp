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

ReloadableTexture::ReloadableTexture(unsigned char *data, int width, int height, bool useMipmap, int padding)
{
    int textureWidth = nextPowerOfTwo(width + padding * 2);
    int textureHeight = nextPowerOfTwo(height + padding * 2);
    auto textureData = new unsigned char[textureWidth * textureHeight](); // ZERO-FILLED
    
    for (int iy = 0; iy < height; iy++)
    {
        for (int ix = 0; ix < width; ix++)
        {
            textureData[(iy + padding) * textureWidth + (ix + padding)] = data[iy * width + ix];
        }
    }
    
    gl::Texture::Format format;
    format.setInternalFormat(GL_ALPHA);
    
    if (useMipmap)
    {
        format.enableMipmapping(true);
        format.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    }
    
    tmp = new gl::Texture(textureData, GL_ALPHA, textureWidth, textureHeight, format);
    delete[] textureData;
}

ReloadableTexture::~ReloadableTexture()
{
    delete tmp;
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
