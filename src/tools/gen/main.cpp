
#include <stdio.h>
#include <iostream>

#include "core/defines.h"
#include "core/commandline/commandline.h"

#include "generators/generatormanager.h"

using namespace c2d::gen;

int main(int argc, char *argv[])
{
   CommandLine cmdline(argc, argv);

   std::cout << "Gen tool - Copyright 2019 - Jeroen Broekhuizen" << std::endl;

   if ( cmdline.size() == 0 )
   {
      // print help information
      std::cout << "No generation command found.";
   }
   else
   {
      const CommandLineArgument& argument = cmdline[0];
      ASSERT(argument.getType() == CommandLineArgument::eCommand);

      Generator* pgenerator = GeneratorManager::getInstance().find(argument.getName());
      if ( pgenerator )
      {
         if ( !pgenerator->generate(cmdline) )
         {
            return -2;
         }
      }
      else
      {
         std::cerr << argument.getName().toUtf8() << " is not a valid command.";
         return -1;
      }
   }

   return 0;
}
