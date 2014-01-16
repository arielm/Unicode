/*
 * THE UNICODE TEST SUITE FOR CINDER: https://github.com/arielm/Unicode
 * COPYRIGHT (C) 2013, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/Unicode/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Timer.h"

#include <iostream>

class Timing
{
    std::ostream &output;
    std::string msg;
    bool started;
    ci::Timer timer;
    
public:
    Timing(std::ostream &output = std::cout, const std::string &msg = "")
    :
    output(output),
    started(false)
    {
        start(msg);
    }
    
    ~Timing()
    {
        stop();
    }
    
    void start(const std::string &msg = "")
    {
        if (!started)
        {
            this->msg = msg;
            
            timer.start();
            started = true;
        }
    }
    
    void stop()
    {
        if (started)
        {
            double tmp = timer.getSeconds();
            started = false;
            
            if (!msg.empty())
            {
                output << msg << ": ";
            }
            
            output << std::setprecision(15) << tmp << std::endl;
        }
    }
};
