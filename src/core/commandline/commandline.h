
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <vector>

#include "core/core_base.h"

#include "commandlineargument.h"

class CORE_API CommandLine
{
   typedef std::vector<CommandLineArgument> Arguments;

public:
   CommandLine(int argc, char *argv[]);

 // query
   int size() const;
   const CommandLineArgument& operator[](int index);

 // searching
   bool                       hasArgument(const String& name) const;
   const CommandLineArgument& getArgument(const String& name) const;
   const CommandLineArgument* findArgument(const String& name) const;

private:
 // parsing
   void parse(int argc, const char* const argv[]);

 // data
   Arguments   mArguments;
};

#endif // COMMAND_LINE_H
