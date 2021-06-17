#pragma once
#include <chrono>

class Timer
{
public:
    Timer();
    double GetMilisecondsElapsed();
    void Restart();

    float TotalTime()const;  // in seconds
    float DeltaTime()const; // in seconds

    void Reset(); // Call before message loop.
    bool Start(); // Call when unpaused.
    bool Stop();  // Call when paused.
    void Tick();  // Call every frame.

private:
    double mSecondsPerCount;
    double mDeltaTime;

    __int64 mBaseTime;
    __int64 mPausedTime;
    __int64 mStopTime;
    __int64 mPrevTime;
    __int64 mCurrTime;

    bool mStopped;

private:
    bool mIsRunnung = false;
#ifdef _WIN32
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> stop;
#else
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> stop;
#endif // _WIN32

};