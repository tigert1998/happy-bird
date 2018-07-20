#include "timer.h"

std::vector<std::chrono::time_point<std::chrono::high_resolution_clock> > Timer::pined_;
// static Timer kSharedTimer();
Timer::TimingId Timer::kFrameTimer = Timer::New();
float Timer::kFrameElapsed = 0;