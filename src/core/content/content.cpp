
#include "content.h"

IContent::IContent():
   mFilename()
{
}

IContent::~IContent()
{
}

// - Get/set
   
const String& IContent::getFilename() const
{
   return mFilename;
}

void IContent::setFilename(const String& name)
{
   mFilename = name;
}
