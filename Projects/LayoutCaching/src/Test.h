#pragma once

#include <boost/bimap.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/set_of.hpp>

#include <string>
#include <memory>

struct Value
{
    std::string v;
    
    Value() {}
    
    Value(const std::string &v)
    :
    v(v)
    {}
    
    ~Value()
    {
        std::cout << "~Value: " << (void*)this << std::endl;
    }
};

class Test
{
public:
    typedef boost::bimaps::bimap<
    boost::bimaps::set_of<int>,
    boost::bimaps::list_of<std::shared_ptr<Value>> // XXX: NOT WORKING WITH unique_ptr
    > container_type;
    
    static void run()
    {
        container_type cache;
        
        cache.insert(typename container_type::value_type(123, std::make_shared<Value>("1.2.3")));
        cache.insert(typename container_type::value_type(456, std::make_shared<Value>("4.5.6")));
        cache.insert(typename container_type::value_type(789, std::make_shared<Value>("7.8.9")));
        
        std::cout << cache.right.begin()->second << " " << cache.right.begin()->first->v << std::endl;
        
        cache.right.erase(cache.right.begin());
        cache.right.erase(cache.right.begin());
        cache.right.erase(cache.right.begin());
        
//        cache.insert(typename container_type::value_type(123, new Value("1.2.3")));
//        cache.insert(typename container_type::value_type(456, new Value("4.5.6")));
//        cache.insert(typename container_type::value_type(789, new Value("7.8.9")));
//        
//        std::cout << cache.right.begin()->second << " " << cache.right.begin()->first->v << std::endl;
//        
//        delete cache.right.begin()->first; cache.right.erase(cache.right.begin());
//        delete cache.right.begin()->first; cache.right.erase(cache.right.begin());
//        delete cache.right.begin()->first; cache.right.erase(cache.right.begin());
        
//        exit(0);
    }
};
