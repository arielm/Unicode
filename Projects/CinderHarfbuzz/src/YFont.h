/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "FreetypeHelper.h"

#include "hb.h"

#include "cinder/gl/Texture.h"

struct FontDescriptor
{
    ci::DataSourceRef source;
    int faceIndex;
    bool forceMemoryLoad;
    
    FontDescriptor(ci::DataSourceRef source, int faceIndex = 0)
    :
    source(source),
    faceIndex(faceIndex),
#if defined(CINDER_ANDROID)
    forceMemoryLoad(true) // HACK IS NECESSARY BECAUSE DataSourceAsset::isFilePath() IS RETURNING TRUE ON ANDROID
#else
    forceMemoryLoad(false)
#endif
    {}
};

struct TextSpan
{
    std::string text;

    hb_script_t script;
    hb_direction_t direction;
    std::string lang; // XXX: NOT CLEAR IF REALLY NECESSARY
    
    TextSpan(const std::string &text, hb_script_t script, hb_direction_t direction, const std::string &lang)
    :
    text(text),
    script(script),
    direction(direction),
    lang(lang)
    {}
};

class YGlyph
{
public:
    ci::gl::TextureRef texture;
    ci::Vec2f offset;
    
    YGlyph(unsigned char *data, int width, int height);
};

class YFont
{
public:
    ci::Vec2f scale;

    float leading;
    float ascent;
    float descent;
    
    YFont(std::shared_ptr<FreetypeHelper> ftHelper, const FontDescriptor &descriptor, float size);
    ~YFont();
    
    void drawSpan(const TextSpan &span, float x, float y) const;

protected:
    std::shared_ptr<FreetypeHelper> ftHelper;
    ci::Buffer memoryBuffer; // ONLY USED WHEN LOADING FROM MEMORY (I.E. ANDROID-ASSET OR WINDOWS-EMBEDDED-RESOURCE)
    FT_Face face;
    
    hb_font_t *hbFont;
    hb_buffer_t *hbBuffer;

    YGlyph* createGlyph(uint32_t codepoint) const;
};
