
#include "commandlineargument.h"

CommandLineArgument::CommandLineArgument():
   mType(eCommand),
   mName(),
   mValue()
{
}

CommandLineArgument::CommandLineArgument(const CommandLineArgument& that):
   mType(that.mType),
   mName(that.mName),
   mValue(that.mValue)
{
}

// - Get/set

CommandLineArgument::Type CommandLineArgument::getType() const
{
   return mType;
}

void CommandLineArgument::setType(Type type)
{
   mType = type;
}

const String& CommandLineArgument::getName() const
{
   return mName;
}

void CommandLineArgument::setName(const String& name)
{
   mName = name;
}

const String& CommandLineArgument::getValue() const
{
   return mValue;
}

void CommandLineArgument::setValue(const String& value)
{
   mType = eArgument;
   mValue = value;
}
