#pragma once
#include "ev.h"
#include "amqpcpp.h"
#include "amqpcpp/libev.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/ThreadPool.h"
#include <Poco/RunnableAdapter.h>
#include <memory>
#include <thread>
#include <chrono>
#include "TimeThread.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Mutex.h"
#include "Poco/AutoPtr.h"

class TCPConnectionNotification : public Poco::Notification
{
public:
    typedef Poco::AutoPtr<TCPConnectionNotification> Ptr;
    TCPConnectionNotification(const std::string &str) :
        m_str(str)
    {
    }

    ~TCPConnectionNotification()
    {
    }
    std::string m_str;
};


class AmqpRequestDispatcher : public Poco::Runnable
{
public:
    AmqpRequestDispatcher(Poco::ThreadPool& threadPool):
        m_threadPool(threadPool) 
    {}

    void run() override
    {
        while (true)
        {
            Poco::Notification::Ptr pNf(m_queue.waitDequeueNotification());
            if (!pNf)
            {
                break;
            }
            TCPConnectionNotification::Ptr pWorkNf = pNf.cast<TCPConnectionNotification>();
            std::cout << "in the dispatcher" << pWorkNf->m_str << std::endl;
        }
        Poco::FastMutex::ScopedLock lock(m_mutex);
    }
    void enqueue(const std::string& str)
    {
        Poco::FastMutex::ScopedLock lock(m_mutex);
        m_queue.enqueueNotification(new TCPConnectionNotification(str));
        if (!m_queue.hasIdleThreads())
        {
         
            try
            {
                m_threadPool.startWithPriority(Poco::Thread::PRIO_LOW, *this);
                std::cout << "frog" << std::endl;
            }
            catch (Poco::Exception&)
            {
                // no problem here, connection is already queued
                // and a new thread might be available later.
            }
        }
    }
private:
    bool _stopped;
    Poco::NotificationQueue         m_queue;
    Poco::ThreadPool &              m_threadPool;
    mutable Poco::FastMutex         m_mutex;
    int _rc;
};

class Receiver : public AMQP::LibEvHandler, public Poco::Runnable, public Poco::RefCountedObject
{
public:
    typedef struct ev_loop EvLoop;
    Receiver(EvLoop *loop)
        : AMQP::LibEvHandler(loop)
        , m_loop(loop)
    {
        m_reconnectFunc = std::bind(&Receiver::Connnect2Rabbitmq, this);
        m_pDispatcher = new AmqpRequestDispatcher(m_pool);
    }
    virtual ~Receiver() = default;
    void publish();
    void receive();
    virtual void run()
    {
        ev_run(m_loop, 0);
    }
    void anotherThread()
    {
        std::cout << "  anotherThread() start" << std::endl;
        //Poco::Thread::sleep(200);
        std::cout << "  anotherThread() " << m_msg<< std::endl;
    }
    void exit()
    {
        m_thread.join();
        DisconnectRabbitmq();
        m_reconnectTimer.stop();
    }
    std::string m_msg;
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
    std::unique_ptr<AMQP::TcpConnection> m_pTcpConnectionX = nullptr;
    std::unique_ptr<AMQP::TcpChannel> m_pTcpChannelX = nullptr;
    EvLoop *m_loop;
    //thread for the event loop
    Poco::Thread m_thread;
    Poco::ThreadPool m_pool;
    AmqpRequestDispatcher* m_pDispatcher;
};
