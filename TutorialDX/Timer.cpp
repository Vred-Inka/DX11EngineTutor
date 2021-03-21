#include "Timer.h"

Timer::Timer()
{
    start = std::chrono::high_resolution_clock::now();
    stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMilisecondsElapsed()
{
    if (mIsRunnung)
    {
        auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
            return elapsed.count();
    }
    else
    {
        auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
        return elapsed.count();
    }
}

void Timer::Restart()
{
    mIsRunnung = true;
    start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
    if (!mIsRunnung)
    {
        return false;
    }
    else
    {
        stop = std::chrono::high_resolution_clock::now();
        mIsRunnung = false;
        return true;
    }
}

bool Timer::Start()
{
    if (mIsRunnung)
    {
        return false;
    }
    else
    {
        start = std::chrono::high_resolution_clock::now();
        mIsRunnung = true;
        return true;
    }
}
