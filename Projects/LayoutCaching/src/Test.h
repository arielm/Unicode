#pragma once

#include <string>
#include <tuple>
#include <iostream>

struct TestObject
{
    std::string s;
    int i;
    
    TestObject(const std::string &s, int i)
    :
    s(s),
    i(i)
    {    }
    
    ~TestObject()
    {
        std::cout << "TestObject DESTROYED: " << s << std::endl;
    }
    
    bool operator<(const TestObject &rhs) const
    {
        return std::tie(s, i) < std::tie(rhs.s, rhs.i);
    }
};

struct TestKey
{
    TestObject o;
    float f;
    
    TestKey(const TestObject &o, float f)
    :
    o(o),
    f(f)
    {}
    
    bool operator<(const TestKey &rhs) const
    {
        return std::tie(o, f) < std::tie(rhs.o, f);
    }
};


class Test
{
public:
    static void run()
    {
        std::map<TestKey, std::string> testMap;
        
        const TestKey k1(TestObject("foo", 123), 0.5f);
        const TestKey k2(TestObject("bar", 123), 0.5f);
        const TestKey k3(TestObject("baz", 123), 0.5f);
        
        /*
         * KEYS ARE COPIED
         */
        testMap[k1] = "hello foo";
        testMap[k2] = "hello bar";
        testMap[k3] = "hello baz";

        /*
         * KEYS ARE NOT COPIED
         */
        testMap.find(k1);
        testMap.find(k2);
        testMap.find(k3);
    }
};
