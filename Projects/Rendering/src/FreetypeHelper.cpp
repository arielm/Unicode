/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "FreetypeHelper.h"

namespace chronotext
{
    namespace zf
    {
        FreetypeHelper::FreetypeHelper()
        {
            FT_Init_FreeType(&library);
        }
        
        FreetypeHelper::~FreetypeHelper()
        {
            FT_Done_FreeType(library);
        }
        
        FT_Library FreetypeHelper::getLib() const
        {
            return library;
        }
    }
}
