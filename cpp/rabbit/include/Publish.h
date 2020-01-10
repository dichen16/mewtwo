#pragma once
#include "ev.h"
#include "amqpcpp.h"
#include "amqpcpp/libev.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include <Poco/RunnableAdapter.h>
#include "Poco/Foundation.h"
#include "Poco/Mutex.h"
#include <memory>
#include <thread>
#include <chrono>
#include "TimeThread.h"

class Publisher : public AMQP::LibEvHandler, public Poco::Runnable, public Poco::RefCountedObject
{
public:
    typedef struct ev_loop EvLoop;
    Publisher(EvLoop *loop)
        : AMQP::LibEvHandler(loop)
        , m_loop(loop)
    {
        m_reconnectFunc = std::bind(&Publisher::Connnect2Rabbitmq, this);
        Connnect2Rabbitmq();
    }
    virtual ~Publisher() = default;
    static Publisher& instance();
    void publish();
    virtual void run()
    {
        std::cout << "before the ev_run(m_loop, 0)" << std::endl;
        ev_run(m_loop, 0);
    }
    void exit()
    {
        m_thread.join();
        DisconnectRabbitmq();
        m_reconnectTimer.stop();
    }
protected:
    bool Connnect2Rabbitmq();
    void DisconnectRabbitmq()
    {
        std::chrono::milliseconds dura(20000);
        std::this_thread::sleep_for(dura);
        m_pTcpChannel->unbindQueue("exchange", "queue", "routingkey");
        //m_pTcpChannel->close();
        m_pTcpConnection->close();
    }
private:
    virtual void onError(AMQP::TcpConnection *connection, const char *message) override
    {
        std::cout << "error: " << message << std::endl;
    }
    virtual void onConnected(AMQP::TcpConnection *connection) override
    {
        std::cout << "connected" << std::endl;
    }
    virtual void onReady(AMQP::TcpConnection *connection) override
    {
        std::cout << "ready" << std::endl;
    }
    virtual void onClosed(AMQP::TcpConnection *connection) override
    {
        std::cout << "closed" << std::endl;
    }
    virtual void onDetached(AMQP::TcpConnection *connection) override
    {

        std::cout << "detached" << std::endl;
    }

    todoBooleanFunc m_reconnectFunc;
    TimerThread m_reconnectTimer;
    std::unique_ptr<AMQP::TcpConnection> m_pTcpConnection = nullptr;
    std::unique_ptr<AMQP::TcpChannel> m_pTcpChannel = nullptr;
    EvLoop *m_loop;
    //thread for the event loop
    Poco::Thread m_thread;
};


class SingletonHolder
    /// This is a helper template class for managing
    /// singleton objects allocated on the heap.
    /// The class ensures proper deletion (including
    /// calling of the destructor) of singleton objects
    /// when the application that created them terminates.
{
public:
    SingletonHolder()
        /// Creates the SingletonHolder.
    {
        std::cout << "build publisher" << std::endl;
        auto *loop = ev_loop_new(0);
        _pS = new Publisher(loop);

    }

    ~SingletonHolder()
        /// Destroys the SingletonHolder and the singleton
        /// object that it holds.
    {
        delete _pS;
    }

    Publisher* get()
        /// Returns a pointer to the singleton object
        /// hold by the SingletonHolder. The first call
        /// to get will create the singleton.
    {
        Poco::FastMutex::ScopedLock lock(_m);
        auto *loop = ev_loop_new(0);
        if (!_pS) _pS = new Publisher(loop);
        return _pS;
    }

    void reset()
        /// Deletes the singleton object.
    {
        Poco::FastMutex::ScopedLock lock(_m);
        delete _pS;
        _pS = 0;
    }

private:
    Publisher * _pS;
    Poco::FastMutex _m;
};