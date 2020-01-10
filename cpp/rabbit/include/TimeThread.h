#pragma once
#pragma once
#include "Poco/Timer.h"
#include <functional>

using todoStatelessFunc = std::function<void()>;
using todoBooleanFunc = std::function<bool()>;

class TimerThread
{
protected:
    enum TIMER_TYPE
    {
        UNIFORM_TIMER,
        PROGRESSIVE_TIMER,
    };
    enum CALLBACK_FUNC_TYPE
    {
        STATELESS_FUNC,
        BOOLEAN_FUNC,
    };
public:
    TimerThread(long startInterval = 0, long periodicInterval = 0)
        : m_timer(startInterval, periodicInterval)
    {}
    void StartUniformTimer(const todoStatelessFunc &func, long startIntervalMs, long periodicIntervalMs)
    {
        m_type = UNIFORM_TIMER;
        m_funcType = STATELESS_FUNC;
        m_todoVoid = func;
        start(startIntervalMs, periodicIntervalMs);
    }
    void StartProgressiveTimer(const todoStatelessFunc &func, int startIntervalSecond = 2, int maxSecond = 256)
    {
        m_type = PROGRESSIVE_TIMER;
        m_funcType = STATELESS_FUNC;
        m_todoVoid = func;
        m_progressive.first = startIntervalSecond;
        m_progressive.second = maxSecond;
        start(m_progressive.first * 1000, m_progressive.first * 1000);
    }
    void StartUniformTimer(const todoBooleanFunc &func, long startIntervalMs, long periodicIntervalMs)
    {
        m_type = UNIFORM_TIMER;
        m_funcType = BOOLEAN_FUNC;
        m_todoBool = func;
        start(startIntervalMs, periodicIntervalMs);
    }
    void StartProgressiveTimer(const todoBooleanFunc &func, int startIntervalSecond = 2, int maxSecond = 256)
    {
        m_type = PROGRESSIVE_TIMER;
        m_funcType = BOOLEAN_FUNC;
        m_todoBool = func;
        m_progressive.first = startIntervalSecond;
        m_progressive.second = maxSecond;
        start(m_progressive.first * 1000, m_progressive.first * 1000);
    }
    void stop()
    {
        m_timer.stop();
    }

protected:
    void start(long startInterval, long periodicInterval)
    {
        m_timer.setStartInterval(startInterval);
        m_timer.setPeriodicInterval(periodicInterval);
        m_timer.start(Poco::TimerCallback<TimerThread>(*this, &TimerThread::onTimer));
    }
    void onTimer(Poco::Timer& timer)
    {
        bool bStop;
        if (m_funcType == STATELESS_FUNC)
        {
            m_todoVoid();
            bStop = false;
        }
        else
        {
            bStop = m_todoBool();
        }
        if (bStop)
        {
            m_timer.restart(0);
        }
        else if (m_type == PROGRESSIVE_TIMER)
        {
            if (m_progressive.first < m_progressive.second)
            {
                m_progressive.first *= 2;
                if (m_progressive.first > m_progressive.second)
                {
                    m_progressive.first = m_progressive.second;
                }
                m_timer.setPeriodicInterval(m_progressive.first * 1000);
            }
        }
    }
private:
    TIMER_TYPE m_type;
    CALLBACK_FUNC_TYPE m_funcType;
    std::pair<int, int> m_progressive;
    Poco::Timer m_timer;
    todoStatelessFunc m_todoVoid;
    todoBooleanFunc m_todoBool;
};
