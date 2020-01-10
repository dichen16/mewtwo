#include "Receive.h"
#include "Publish.h"
#include <string>
#include <memory>
#include <chrono>
#include <ctime>


int main(int argc, char* argv[])
{
    auto *loop1 = ev_loop_new(0);
    std::unique_ptr<Receiver> handler1 = std::unique_ptr<Receiver>(new Receiver(loop1));
    handler1->receive();

    auto now = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
    std::cout << std::ctime(&end_time) << std::endl;

    Publisher::instance().publish();
    while (true)
    {
    }
    return 0;
}


//#include "Poco/ThreadPool.h"
//#include "Poco/Runnable.h"
//#include <iostream>
//class HelloRunnable : public Poco::Runnable
//{
//    virtual void run()
//    {
//        std::cout << "Hello, world!" << std::endl;
//    }
//};
//int main(int argc, char** argv)
//{
//    HelloRunnable runnable1;
//    HelloRunnable runnable2;
//
//    std::chrono::milliseconds dura(5000);
//    Poco::ThreadPool pool;
//    //Poco::ThreadPool::defaultPool().start(runnable);
//    pool.start(runnable1); 
//    pool.start(runnable2);
//    std::this_thread::sleep_for(dura);
//    pool.joinAll();
//    return 0;
//}