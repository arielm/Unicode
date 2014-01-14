/*
 * TRYING TO OPTIMIZE LayoutCache
 * SEE http://stackoverflow.com/questions/21112208/how-to-avoid-key-copying-when-using-boost-bimap
 */

#pragma once

#include <string>
#include <iostream>

#include <boost/bimap.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/set_of.hpp>

class Test
{
public:
    struct ComplexKey
    {
        std::string text;
        int dummy;
        
        ComplexKey(const std::string &text, int dummy) : text(text), dummy(dummy) {}
        
        ~ComplexKey()
        {
            std::cout << "~ComplexKey " << (void*)this << " " << text << std::endl;
        }
        
        bool operator<(const ComplexKey &rhs) const
        {
            return tie(text, dummy) < tie(rhs.text, rhs.dummy);
        }
    };
    
    typedef boost::bimaps::bimap<
    boost::bimaps::set_of<ComplexKey>,
    boost::bimaps::list_of<std::string>
    > container_type;
    
    container_type cache;
    
    void run()
    {
        getValue("foo", 123); // 3 COPIES
        getValue("bar", 456); // 3 COPIES
        getValue("foo", 123); // 2 COPIES
    }
    
    std::string getValue(const std::string &text, int dummy)
    {
        const ComplexKey key(text, dummy); // COPY #1 OF text
        auto it = cache.left.find(key); // COPY #2 OF text (BECAUSE key IS COPIED)
        
        if (it != cache.left.end())
        {
            return it->second;
        }
        else
        {
            auto value = std::to_string(text.size()) + "." + std::to_string(dummy); // WHATEVER...
            cache.insert(typename container_type::value_type(key, value)); // COPY #3 OF text
            
            return value;
        }
    }
};
