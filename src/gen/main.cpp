
#include <stdio.h>

#include "core/defines.h"
#include "core/commandline/commandline.h"
#include "core/vfs/filesystem.h"

#include "generators/projectgenerator.h"

int main(int argc, char *argv[])
{
   CommandLine cmdline(argc, argv);

   if ( cmdline.size() == 0 )
   {
      // print help information
   }
   else
   {
      const CommandLineArgument& argument = cmdline[0];
      ASSERT(argument.getType() == CommandLineArgument::eCommand);

      FileSystem& filesystem = FileSystem::getInstance();
      filesystem.addPath("../scripts");

      Generator* pgenerator = NULL;
      if ( argument.getName() == String("project") )
      {
         pgenerator = new ProjectGenerator();
      }

      pgenerator->setFileSystem(filesystem);
      pgenerator->generate(cmdline);
   }

   return 0;
}
