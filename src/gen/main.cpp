
#include <stdio.h>
#include <iostream>

#include "core/defines.h"
#include "core/commandline/commandline.h"
#include "core/commandline/commandlineexception.h"

#include "generators/projectgenerator.h"
#include "generators/modulegenerator.h"
#include "generators/classgenerator.h"

int main(int argc, char *argv[])
{
   CommandLine cmdline(argc, argv);

   std::cout << "Gen tool - Copyright 2014 - Jeroen Broekhuizen" << std::endl;

   if ( cmdline.size() == 0 )
   {
      // print help information
      std::cout << "Invalid arguments.";
   }
   else
   {
      const CommandLineArgument& argument = cmdline[0];
      ASSERT(argument.getType() == CommandLineArgument::eCommand);

      Generator* pgenerator = NULL;
      if ( argument.getName() == UTEXT("project") )
      {
         pgenerator = new ProjectGenerator();
      }
      else if ( argument.getName() == UTEXT("mod") )
      {
         pgenerator = new ModuleGenerator();
      }
      else if ( argument.getName() == UTEXT("class") )
      {
         pgenerator = new ClassGenerator();
      }
      else
      {
         std::cerr << argument.getName().toUtf8() << " is not a valid target.";
         return -1;
      }

      try
      {
         if ( !pgenerator->generate(cmdline) )
         {
            return -2;
         }
      }
      catch ( CommandLineException& e )
      {
         std::cerr << e.getMessage().toUtf8();
         return -2;
      }
   }

   return 0;
}
