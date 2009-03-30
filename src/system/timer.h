#pragma once

#ifdef WIN32

#include <windows.h>

class Timer
{
public:
   Timer();

   void   begin();
   double getInterval();

private:
   LARGE_INTEGER mFreq;
   LARGE_INTEGER mStart;
};

#else

class Timer
{
public:
   Timer();
   ~Timer();

   void begin();
   double getInterval();

private:
   double getTicks();

   double mStart;
};

#endif
