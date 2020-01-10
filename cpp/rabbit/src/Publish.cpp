#include "Publish.h"
#include "Poco/SingletonHolder.h"

namespace
{
    static SingletonHolder sh;
}

Publisher& Publisher::instance()
{
    return *sh.get();
}


void Publisher::publish()
{
    while (true)
    {
        std::chrono::milliseconds dura(100);
        std::this_thread::sleep_for(dura);
        m_pTcpChannel->publish("exchange", "routingkey", "my-payload");
        std::cout << "sent" << std::endl;
    }
    
}

bool Publisher::Connnect2Rabbitmq()
{
    m_pTcpConnection = std::unique_ptr<AMQP::TcpConnection>(
        new AMQP::TcpConnection(this, AMQP::Address("amqp://admin:admin@localhost:5672")));
    m_pTcpChannel = std::unique_ptr<AMQP::TcpChannel>(new AMQP::TcpChannel(m_pTcpConnection.get()));
    m_pTcpChannel->onReady([]() {
        std::cout << "publish channel on ready " << std::endl;
    });
    m_pTcpChannel->onError([this](const char *message) {
        std::cout << "publish channel on error " << message << std::endl;
        m_pTcpConnection.release();
        m_pTcpChannel.release();
        m_reconnectTimer.stop();
        m_reconnectTimer.StartProgressiveTimer(m_reconnectFunc, 2, 64);
    });
    std::cout << "before them_thread.start(*this)" << std::endl;
    m_thread.start(*this);
    std::cout << "publish thread detached" << std::endl;
    return true;
}
