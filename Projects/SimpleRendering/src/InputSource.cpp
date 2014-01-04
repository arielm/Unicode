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

const string URI_FILE = "file://";
const string URI_ASSETS = "assets://";
const string URI_RES = "res://";

fs::path InputSource::getFilePath(const std::string &uri)
{
    fs::path filePath;
    
    if (boost::starts_with(uri, URI_FILE))
    {
        filePath = uri.substr(URI_FILE.size());
    }
    else if (boost::starts_with(uri, URI_ASSETS))
    {
        filePath = getAssetPath(uri.substr(URI_ASSETS.size()));
    }
#if defined(CINDER_COCOA)
    else if (boost::starts_with(uri, URI_RES))
    {
        filePath = App::getResourcePath(uri.substr(URI_RES.size()));
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

DataSourceRef InputSource::getDataSource(const string &uri)
{
    if (boost::starts_with(uri, URI_FILE))
    {
        return DataSourcePath::create(uri.substr(URI_FILE.size()));
    }
    else if (boost::starts_with(uri, URI_ASSETS))
    {
        return DataSourcePath::create(getAssetPath(uri.substr(URI_FILE.size()))); // TODO: SPECIAL VERSION REQUIRED FOR ANDROID
    }
    else if (boost::starts_with(uri, URI_RES))
    {
        return loadResource(uri.substr(URI_RES.size())); // TODO: SPECIAL VERSION REQUIRED FOR WINDOWS
    }
    
    return DataSourceRef();
}
