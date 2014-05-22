
#ifndef RANGE_H
#define RANGE_H

#include <cstdlib>

#include "core/core_base.h"

class DataStream;

class CORE_API Range
{
public:
   Range(): mStart(0.0f), mRange(0.0f) {}
   Range(float start, float range): mStart(start), mRange(range) {}

 // query
   float getRandom() const {
      return mStart + rand() / (float)RAND_MAX * mRange;
   }

 // operaitons
   inline void set(float start, float range) {
      mStart = start;
      mRange = range;
   }

private:
   friend CORE_API DataStream& operator<<(DataStream& stream, const Range& range);
   friend CORE_API DataStream& operator>>(DataStream& stream, Range& range);

 // data
   float mStart;
   float mRange;
};

#endif // RANGE_H
