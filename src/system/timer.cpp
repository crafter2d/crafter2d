#include "timer.h"

#ifdef WIN32

Timer::Timer():
   mFreq(),
   mStart()
{
   QueryPerformanceFrequency(&mFreq);
}

void Timer::begin()
{
   QueryPerformanceCounter(&mStart);
}

double Timer::getInterval()
{
   LARGE_INTEGER end;
   QueryPerformanceCounter(&end);
   return (double)(end.QuadPart - mStart.QuadPart) / (double)mFreq.QuadPart;
}

#else

Timer::Timer()
{
}

void Timer::begin()
{
   mStart = getTicks();
}

double Timer::getInterval()
{
   double end = getTicks();
   return end - mStart;
}

double Timer::getTicks()
{
   return ((double)SDL_GetTicks()) / 1000.0;
}

#endif
