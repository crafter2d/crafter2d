
#include "range.h"

#include "core/streams/datastream.h"

DataStream& operator<<(DataStream& stream, const Range& range)
{
   stream << range.mStart << range.mRange;
   return stream;
}

DataStream& operator>>(DataStream& stream, Range& range)
{
   stream >> range.mStart >> range.mRange;
   return stream;
}
