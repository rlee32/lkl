#pragma once

// A nanosecond-accurate timer, that works like a stopwatch.
// Call start() to start counting, then stop() to get the elapsed time.
//
// Example usage:
// Timer timer;
// timer.start();
// auto elapsed_time = timer.stop();

#include <chrono>
#include <cstdint>

class NanoTimer
{
public:
    inline void start()
    {
        startTime = std::chrono::high_resolution_clock::now();
    }
    inline uint64_t stop() const
    {
        return (std::chrono::high_resolution_clock::now() - startTime).count();
    }

private:
  using TimePoint = std::chrono::high_resolution_clock::time_point;
  TimePoint startTime;
};

