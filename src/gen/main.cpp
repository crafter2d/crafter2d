
#include <stdio.h>
#include <iostream>

#include "core/defines.h"
#include "core/commandline/commandline.h"
#include "core/vfs/filesystem.h"

#include "generators/projectgenerator.h"
#include "generators/modulegenerator.h"

int main(int argc, char *argv[])
{
   CommandLine cmdline(argc, argv);

   std::cout << "Gen tool - Copyright 2012 - Jeroen Broekhuizen" << std::endl;

   if ( cmdline.size() == 0 )
   {
      // print help information
      std::cout << "Invalid arguments.";
   }
   else
   {
      const CommandLineArgument& argument = cmdline[0];
      ASSERT(argument.getType() == CommandLineArgument::eCommand);

      FileSystem& filesystem = FileSystem::getInstance();
      filesystem.addPath(UTEXT("../scripts"));

      Generator* pgenerator = NULL;
      if ( argument.getName() == UTEXT("project") )
      {
         pgenerator = new ProjectGenerator();
      }
      else if ( argument.getName() == UTEXT("mod") )
      {
         pgenerator = new ModuleGenerator();
      }
      else
      {
         std::cerr << argument.getName().toUtf8() << " is not a valid target.";
         return 1;
      }

      pgenerator->setFileSystem(filesystem);
      pgenerator->generate(cmdline);
   }

   return 0;
}
