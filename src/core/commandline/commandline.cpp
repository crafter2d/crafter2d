
#include "commandline.h"

#include "core/defines.h"
#include "core/string/string.h"

#include "commandlineexception.h"

CommandLine::CommandLine(int argc, char *argv[]):
   mArguments()
{
   parse(argc, argv);
}

// - Query

int CommandLine::size() const
{
   return mArguments.size();
}

const CommandLineArgument& CommandLine::operator[](int index)
{
   return mArguments[index];
}

// - Searching

bool CommandLine::hasArgument(const String& name) const
{
   return findArgument(name) != NULL;
}

const CommandLineArgument& CommandLine::getArgument(const String& name) const
{
   const CommandLineArgument* presult = findArgument(name);
   if ( presult == NULL )
   {
      throw new CommandLineException();
   }
   return *presult;
}

const CommandLineArgument* CommandLine::findArgument(const String& name) const
{
   for ( std::size_t index = 0; index < mArguments.size(); index++ )
   {
      const CommandLineArgument& arg = mArguments[index];
      if ( arg.getName() == name )
      {
         return &arg;
      }
   }
   return NULL;
}

// - Parsing

void CommandLine::parse(int argc, const char* const argv[])
{
   // argv[0] = executable itself
   //      1  = command
   //     >1  = arguments for the command

   if ( argc > 1 )
   {
      for ( int index = 1; index < argc; index++ )
      {
         const char* parg = argv[index];
         String inputarg(parg);
         CommandLineArgument argument;

         int pos = inputarg.indexOf('=');
         if ( pos == -1 )
         {
            argument.setName(String(argv[index]));

            if ( index+1 < argc )
            {
               // not in current argument, check if next starts with assignment
               if ( strncmp(argv[index+1], "=", 1) == 0 )
               {
                  if ( strlen(argv[index+1]) > 1 )
                  {
                     String valuestr(argv[index+1]);
                     argument.setValue(valuestr.subStr(1, valuestr.length() - 1));
                  }
                  else
                  {
                     // only '=' argument, so take the next argument as value
                     if ( index + 2 < argc )
                     {
                        argument.setValue(String(argv[index+2]));
                     }
                     else
                     {
                        // value is empty
                     }
                  }
               }
            }
         }
         else
         {
            // in there
            argument.setName(inputarg.subStr(0, pos));
            argument.setValue(inputarg.subStr(pos+1, inputarg.length() - pos));
         }

         mArguments.push_back(argument);
      }
   }
}
