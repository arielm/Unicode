/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "GlyphData.h"
#include "ReloadableTexture.h"

#include "chronotext/InputSource.h"

#include "hb.h"

#include <map>

class ActualFont
{
public:
    struct Descriptor
    {
        chr::InputSourceRef source;
        int faceIndex;
        float scale;
        bool forceMemoryLoad;
        
        Descriptor()
        {}
        
        Descriptor(chr::InputSourceRef source, int faceIndex = 0, float scale = 1, bool forceMemoryLoad = false)
        :
        source(source),
        faceIndex(faceIndex),
        scale(scale),
        forceMemoryLoad(forceMemoryLoad)
        {}
        
        bool empty()
        {
            return !bool(source);
        }
    };
    
    struct Key
    {
        std::string uri;
        int faceIndex;
        float baseSize;
        bool useMipmap;
        
        Key(const Descriptor &descriptor, float baseSize, bool useMipmap)
        :
        uri(descriptor.source->getURI()),
        faceIndex(descriptor.faceIndex),
        baseSize(baseSize * descriptor.scale),
        useMipmap(useMipmap)
        {}
        
        bool operator<(const Key &rhs) const
        {
            return tie(uri, faceIndex, baseSize, useMipmap) < tie(rhs.uri, rhs.faceIndex, rhs.baseSize, rhs.useMipmap);
        }
    };

    struct Glyph
    {
        ReloadableTexture *texture;
        ci::Vec2f offset;
        ci::Vec2f size;
        
        float tx1;
        float ty1;
        float tx2;
        float ty2;
        
        Glyph()
        :
        texture(NULL)
        {}
        
        Glyph(ReloadableTexture *texture, ci::Vec2f offset, ci::Vec2f size)
        :
        texture(texture),
        offset(offset),
        size(size)
        {
            tx1 = 0;
            ty1 = 0;
            tx2 = size.x / texture->getWidth();
            ty2 = size.y / texture->getHeight();
        }
    };
    
    struct Metrics
    {
        float height;
        float ascent;
        float descent;
        float strikethroughOffset;
        float underlineOffset;
        float lineThickness;
        
        Metrics& operator*(float s)
        {
            height *= s;
            ascent *= s;
            descent *= s;
            strikethroughOffset *= s;
            underlineOffset *= s;
            lineThickness *= s;
            
            return *this;
        }
    };

    ~ActualFont(); // MUST BE PUBLIC BECAUSE OF shared_ptr

protected:
    std::shared_ptr<FreetypeHelper> ftHelper;

    Descriptor descriptor;
    float baseSize;
    bool useMipmap;
    int padding;

    ci::Vec2f scale;
    Metrics metrics;
    bool loaded;

    ci::Buffer memoryBuffer;
    FT_Face ftFace;
    hb_font_t *hbFont;
    
    std::map<uint32_t, std::unique_ptr<Glyph>> glyphCache;
    std::vector<std::unique_ptr<ReloadableTexture>> standaloneTextures;

    ActualFont(std::shared_ptr<FreetypeHelper> ftHelper, const Descriptor &descriptor, float baseSize, bool useMipmap);
    
    void reload();
    void unload();
    void discardTextures();
    size_t getTextureMemoryUsage() const;
    
    Glyph* getGlyph(uint32_t codepoint);
    Glyph* createGlyph(uint32_t codepoint);

    std::string getFullName();

    friend class FontManager;
    friend class VirtualFont;
};
