
#include "contentheader.h"

#include "core/streams/datastream.h"

ContentHeader::ContentHeader():
   mModule(),
   mName()
{
}

// - Get/set

const String& ContentHeader::getName() const
{
   return mName;
}

void ContentHeader::setName(const String& name)
{
   mName = name;
}

const String& ContentHeader::getModuleName() const
{
   return mModule;
}

void ContentHeader::setModuleName(const String& name)
{
   mModule = name;
}

// - Read/write

bool ContentHeader::read(DataStream& stream)
{
   stream << mName << mModule;
   return true;
}
   
bool ContentHeader::write(DataStream& stream)
{
   stream << mName << mModule;
   return true;
}
