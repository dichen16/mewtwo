#pragma once
#include "Poco/ThreadPool.h"
#include "Poco/Runnable.h"
#include <iostream>

class HelloRunnable : public Poco::Runnable
{
    virtual void run()
    {
        std::cout << "Hello, world!" << std::endl;
    }
};
