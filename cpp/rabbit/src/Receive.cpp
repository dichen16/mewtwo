#include "Receive.h"

void Receiver::publish()
{
    //m_pTcpConnectionX = std::unique_ptr<AMQP::TcpConnection>(
    //    new AMQP::TcpConnection(this, AMQP::Address("amqp://admin:admin@localhost:5672")));
    /*m_pTcpChannelX = std::unique_ptr<AMQP::TcpChannel>(new AMQP::TcpChannel(m_pTcpConnection.get()));*/
    //m_thread.start(*this);
    while (true)
    {
        std::chrono::milliseconds dura(1000);
        std::this_thread::sleep_for(dura);
        m_pTcpChannelX->publish("exchange", "routingkey", "my-payload");
    }
}

void Receiver::receive()
{
    m_pTcpConnection = std::unique_ptr<AMQP::TcpConnection>(
        new AMQP::TcpConnection(this, AMQP::Address("amqp://admin:admin@localhost:5672")));
    m_pTcpChannel = std::unique_ptr<AMQP::TcpChannel>(new AMQP::TcpChannel(m_pTcpConnection.get()));
    m_pTcpChannel->onReady([]() {
        std::cout << "receive channel on ready " << std::endl;
    });
    m_pTcpChannel->onError([this](const char *message) {
        std::cout << "receive channel on error " << message << std::endl;
        //m_pTcpConnection.release();
        //m_pTcpChannel.release();
        //m_reconnectTimer.stop();
        //m_reconnectTimer.StartProgressiveTimer(m_reconnectFunc, 2, 64);
    });
   
    m_thread.start(*this);

    m_pTcpChannel->declareExchange("exchange", AMQP::direct, AMQP::autodelete)
        .onError([](const char *message) {
        std::cout << "declareExchange on error" << message << std::endl;
    })
        .onSuccess([]() {
        std::cout << "declareExchange on success" << std::endl;
    });
    m_pTcpChannel->declareQueue("queue", AMQP::autodelete)
        .onError([](const char *message) {
        std::cout << "declareQueue on error" << message << std::endl;
    })
        .onSuccess([this]() {
        std::cout << "declareQueue on success" << std::endl;
        m_pTcpChannel->bindQueue("exchange", "queue", "routingkey");
    });
    auto messageCb = [this](
        const AMQP::Message &message, uint64_t deliveryTag,
        bool redelivered)
    {
        //assign task by 
        message.routingkey();
        //use thread pool to execute corresponding process 
        std::string str = message.body();
        m_pDispatcher->enqueue(message.body());

        std::cout << "The message is " << str << std::endl;
    };
    auto startCb = [](const std::string &consumertag) {

        std::cout << "consume operation started: " << consumertag << std::endl;
    };
    auto errorCb = [](const char *message) {

        std::cout << "consume operation failed" << std::endl;
    };
    m_pTcpChannel->consume("queue")
        .onReceived(messageCb)
        .onSuccess(startCb)
        .onError(errorCb);
}

bool Receiver::Connnect2Rabbitmq()
{
    m_pTcpConnection = std::unique_ptr<AMQP::TcpConnection>(
       new AMQP::TcpConnection(this, AMQP::Address("amqp://admin:admin@localhost:5672")));
    m_pTcpChannel = std::unique_ptr<AMQP::TcpChannel>(new AMQP::TcpChannel(m_pTcpConnection.get()));
    m_pTcpChannel->onReady([]() {
       std::cout << "channel on ready " << std::endl;
    });
    m_pTcpChannel->onError([this](const char *message) {
       std::cout << "channel on error " << message << std::endl;
       //m_pTcpConnection.release();
       //m_pTcpChannel.release();
       //m_reconnectTimer.stop();
       //m_reconnectTimer.StartProgressiveTimer(m_reconnectFunc, 2, 64);
    });
    m_pTcpChannel->declareExchange("exchange", AMQP::direct, AMQP::autodelete)
       .onError([](const char *message) {
       std::cout << "declareExchange on error" << message << std::endl;
    })
       .onSuccess([]() {
       std::cout << "declareExchange on success" << std::endl;
    });
    m_pTcpChannel->declareQueue("queue", AMQP::autodelete)
       .onError([](const char *message) {
       std::cout << "declareQueue on error" << message << std::endl;
    })
       .onSuccess([this]() {
       std::cout << "declareQueue on success" << std::endl;
       m_pTcpChannel->bindQueue("exchange", "queue", "routingkey");
    });
    auto messageCb = [this](
       const AMQP::Message &message, uint64_t deliveryTag,
       bool redelivered)
    {
       m_pDispatcher->enqueue(message.body());
       
    };
    auto startCb = [](const std::string &consumertag) {

       std::cout << "consume operation started: " << consumertag << std::endl;
    };
    auto errorCb = [](const char *message) {

       std::cout << "consume operation failed" << std::endl;
    };
    m_pTcpChannel->consume("queue")
       .onReceived(messageCb)
       .onSuccess(startCb)
       .onError(errorCb);
    //m_thread.start(*this);
    return true;
}


