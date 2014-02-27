
#include "worldreader.h"

#include "core/streams/datastream.h"
#include "core/string/string.h"
#include "core/defines.h"

IContent* WorldReader::read(DataStream& stream)
{
   String name;
   stream.readString(name);
   return NULL;
}
