
#include "vectorrange.h"

#include "core/streams/datastream.h"

DataStream& operator<<(DataStream& stream, const VectorRange& range)
{
   stream << range.mStart << range.mRange;
   return stream;
}

DataStream& operator>>(DataStream& stream, VectorRange& range)
{
   stream >> range.mStart >> range.mRange;
   return stream;
}
