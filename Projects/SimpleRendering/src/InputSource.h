/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/DataSource.h"

class InputSource
{
public:
    static ci::fs::path getFilePath(const std::string &uri);
    static ci::DataSourceRef getDataSource(const std::string &uri);
};
