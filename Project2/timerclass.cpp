#include "timerclass.h"


TimerClass::TimerClass()
{
}


TimerClass::TimerClass(const TimerClass& other)
{
}


TimerClass::~TimerClass()
{
}

bool TimerClass::Initialize()
{
    INT64 frequency;


    // Get the cycles per second speed for this system.
    // 성능 측정 함수
    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    if (frequency == 0)
    {
        return false;
    }

    // Store it in floating point.
    m_frequency = (float)frequency;

    // Get the initial start time.
    // 시작 지점의 시간
    QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

    return true;
}

void TimerClass::Frame()
{
    INT64 currentTime;
    INT64 elapsedTicks;


    // Query the current time.
    // 프레임이 시작될때의 클럭 수
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

    // Calculate the difference in time since the last time we queried for the current time.
    // 이전 프레임과 현제 프레임의 클럭 수 
    elapsedTicks = currentTime - m_startTime;

    // Calculate the frame time.
    // 프레임 사이의 클럭 수 차이를 주파수로 나누어서 프레임 사이의 second를 구함
    m_frameTime = (float)elapsedTicks / m_frequency;

    // Restart the timer.
    m_startTime = currentTime;

    return;
}

float TimerClass::GetTime()
{
    return m_frameTime;
}