/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#include "InputSource.h"

#include "cinder/app/App.h"
#include "cinder/Utilities.h"

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace app;

const string REF_FILE = "file://";
const string REF_ASSETS = "assets://";
const string REF_RES = "res://";

fs::path InputSource::getFilePath(const std::string &ref)
{
    fs::path filePath;
    
    if (boost::starts_with(ref, REF_FILE))
    {
        filePath = ref.substr(REF_FILE.size());
    }
    else if (boost::starts_with(ref, REF_ASSETS))
    {
        filePath = getAssetPath(ref.substr(REF_ASSETS.size()));
    }
#if defined(CINDER_COCOA)
    else if (boost::starts_with(ref, REF_RES))
    {
        filePath = App::getResourcePath(ref.substr(REF_RES.size()));
    }
#endif
    
    if (fs::exists(filePath))
    {
        return filePath;
    }
    else
    {
        return "";
    }
}

DataSourceRef InputSource::getDataSource(const string &ref)
{
    if (boost::starts_with(ref, REF_FILE))
    {
        return DataSourcePath::create(ref.substr(REF_FILE.size()));
    }
    else if (boost::starts_with(ref, REF_ASSETS))
    {
        return DataSourcePath::create(getAssetPath(ref.substr(REF_FILE.size()))); // TODO: SPECIAL VERSION REQUIRED FOR ANDROID
    }
    else if (boost::starts_with(ref, REF_RES))
    {
        return loadResource(ref.substr(REF_RES.size())); // TODO: SPECIAL VERSION REQUIRED FOR WINDOWS
    }
    
    return DataSourceRef();
}
