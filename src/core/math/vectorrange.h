
#ifndef VECTOR_RANGE_H
#define VECTOR_RANGE_H

#include <cstdlib>

#include "core/core_base.h"

#include "vector.h"

class CORE_API VectorRange
{
public:

   VectorRange(): mStart(), mRange() {}
   VectorRange(const Vector& start, const Vector& range): mStart(start), mRange(range) {}

 // query
   inline Vector getRandomVector() {
      return Vector(mStart.x + rnd() * mRange.x, mStart.y + rnd() * mRange.y);
   }

 // operations
   inline void set(const Vector& left, const Vector& right) {
      mStart = left;
      mRange = right - left;
   }

private:
   friend CORE_API DataStream& operator<<(DataStream& stream, const VectorRange& range);
   friend CORE_API DataStream& operator>>(DataStream& stream, VectorRange& range);

 // operations
   inline float rnd() {
      return rand() / (float)RAND_MAX;
   }

  // data
   Vector mStart;
   Vector mRange;
};

#endif // VECTOR_RANGE_H
