
#ifndef COMMAND_LINE_ARGUMENT_H
#define COMMAND_LINE_ARGUMENT_H

#include "core/core_base.h"
#include "core/string/string.h"

class CORE_API CommandLineArgument
{
public:
   enum Type
   {
      eCommand,
      eArgument
   };

   CommandLineArgument();
   CommandLineArgument(const CommandLineArgument& that);

 // get/set
   Type          getType() const;
   void          setType(Type type);

   const String& getName() const;
   void          setName(const String& name);

   const String& getValue() const;
   void          setValue(const String& value);

private:
   
   String mName;
   String mValue;
   Type   mType;
};

#endif // COMMAND_LINE_ARGUMENT_H
