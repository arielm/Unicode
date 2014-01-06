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

typedef std::vector<std::shared_ptr<class YFont>> FontList;

struct FontDescriptor
{
    ci::DataSourceRef source;
    int faceIndex;
    
    FontDescriptor(ci::DataSourceRef source, int faceIndex = 0)
    :
    source(source),
    faceIndex(faceIndex)
    {}
};

struct YGlyph
{
    ci::gl::TextureRef texture;
    ci::Vec2f offset;
    
    YGlyph(unsigned char *data, int width, int height);
};

class YFont
{
public:
    ci::Vec2f scale;
    hb_font_t *hbFont;
    
    float leading;
    float ascent;
    float descent;
    
    YFont(std::shared_ptr<FreetypeHelper> ftHelper, const FontDescriptor &descriptor, float size);
    ~YFont();
    
    YGlyph* getGlyph(uint32_t codepoint);
    void clearCache();
    
protected:
    std::shared_ptr<FreetypeHelper> ftHelper;
    FT_Face ftFace;
    
    std::map<uint32_t, std::unique_ptr<YGlyph>> cache;
    
    YGlyph* createGlyph(uint32_t codepoint) const;
};
