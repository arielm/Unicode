/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "FreetypeHelper.h"

#include "cinder/gl/Texture.h"

#include "hb.h"

#include <map>

class ActualFont
{
public:
    struct Glyph
    {
        ci::gl::Texture* texture;
        ci::Vec2f offset;
        
        Glyph(ci::gl::Texture *texture, ci::Vec2f offset)
        :
        texture(texture),
        offset(offset)
        {}
    };

    ci::Vec2f scale;
    hb_font_t *hbFont;
    
    float leading;
    float ascent;
    float descent;
    
    ActualFont(std::shared_ptr<FreetypeHelper> ftHelper, const ci::fs::path &filePath, float size);
    ~ActualFont();
    
    Glyph* getGlyph(uint32_t codepoint);
    void clearGlyphCache();
    
protected:
    std::shared_ptr<FreetypeHelper> ftHelper;
    FT_Face ftFace;
    
    std::map<uint32_t, Glyph*> glyphCache;
    std::vector<ci::gl::TextureRef> textureList;
    
    Glyph* createGlyph(uint32_t codepoint);
    ci::gl::Texture* createTexture(unsigned char *data, int width, int height);
};
