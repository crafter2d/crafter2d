
#include "modulegenerator.h"

#include <ctemplate/template.h>
#include <string>
#include <iostream>

#include "core/commandline/commandline.h"
#include "core/vfs/stdiofile.h"

bool ModuleGenerator::generate(CommandLine& commandline)
{
   std::string baseHeader, baseClass, name, init;

   // see what type of module to create
   const CommandLineArgument* parg = commandline.findArgument(UTEXT("type"));
   if ( parg != NULL )
   {
      if ( parg->getValue() == UTEXT("content") )
      {
         parg = commandline.findArgument(UTEXT("supports"));
         if ( parg != NULL )
         {
            std::string exts = parg->getValue().toUtf8();
            init = "pmodule->setSupportedFiles(UTEXT(\"" + exts + "\"));";
         }

         baseHeader = "modules/contentmodule.h";
         baseClass = "ContentModule";
      }
   }
   else
   {
      std::cerr << "Missing required argument 'type'.";
      return false;
   }

   // get the module name
   parg = commandline.findArgument(UTEXT("name"));
   if ( parg != NULL )
   {
      name = parg->getValue().toUtf8();
   }
   else
   {
      std::cerr << "Missing required argument 'name'.";
      return false;
   }

   // generate the file content
   ctemplate::TemplateDictionary dict("values");
   dict.SetValue("BASE_HEADER", baseHeader);
   dict.SetValue("BASE_CLASS", baseClass);
   dict.SetValue("NAME", name);
   dict.SetValue("INIT", init);

   std::string output;
   ctemplate::ExpandTemplate("../projects/templates/mod_content.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);

   // for now we create the module in the crafter2d/src/mods folder
   StdioFile file;
   String filename = UTEXT("../src/mods/mod_") + String(name) + UTEXT(".cpp");
   if ( file.open(filename, File::EWrite | File::EText) )
   {
      file.write(String(output));
      file.close();

      return true;
   }
   else
   {
      std::cerr << "Could not write to file " << filename.toUtf8();
      return false;
   }

   return false;
}
