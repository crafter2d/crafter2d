
#include "childdefinitionproto.h"

#include "core/streams/datastream.h"

void ChildDefinitionProto::read(DataStream& stream)
{
   stream >> mID >> (int&) mRefType >> mRef >> mOffset.x >> mOffset.y;
}

void ChildDefinitionProto::write(DataStream& stream) const
{
   stream << mID << (int) mRefType << mRef << mOffset.x << mOffset.y;
}
