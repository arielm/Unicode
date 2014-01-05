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

using namespace std;
using namespace ci;
using namespace app;

fs::path InputSource::getFilePath(const std::string &input)
{
    auto uri = decodeURI(input);
    fs::path filePath;
    
    if (uri.scheme == "file")
    {
        filePath = uri.path;
    }
    else if (uri.scheme == "assets")
    {
        filePath = getAssetPath(uri.path);
    }
    else if (uri.scheme == "res")
    {
#if defined(CINDER_COCOA)
        filePath = App::getResourcePath(uri.path);
#endif
    }
    
    if (fs::exists(filePath))
    {
        return filePath;
    }
    else
    {
        return "";
    }
}

DataSourceRef InputSource::getDataSource(const string &input)
{
    auto uri = decodeURI(input);
    
    if (uri.scheme == "file")
    {
        return DataSourcePath::create(uri.path);
    }
    else if (uri.scheme == "assets")
    {
        return DataSourcePath::create(getAssetPath(uri.path)); // WE'RE TALKING ABOUT "CINDER ASSETS", NOT "ANDROID ASSETS"
    }
    else if (uri.scheme == "res")
    {
        return loadResource(uri.path); // TODO: SPECIAL VERSIONS REQUIRED FOR WINDOWS (EMBEDDED RESOURCES) AND ANDROID (ASSETS)
    }
    
    return DataSourceRef();
}

InputSource::URI InputSource::decodeURI(const std::string &input)
{
    InputSource::URI output;
    auto found = input.find("://");
    
    if (found != string::npos)
    {
        output.scheme = input.substr(0, found);
        output.path = input.substr(found + 3);
    }
    
    return output;
}
