
#include "modulegenerator.h"

#include <ctemplate/template.h>
#include <string>
#include <iostream>

#include "core/commandline/commandline.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/stdiofile.h"

namespace c2d::gen
{
   GENERATOR_IMPL(ModuleGenerator);

   UChar ModuleGenerator::getBriefCommand() const
   {
      return L'm';
   }

   const String& ModuleGenerator::getCommand() const
   {
      static const String sCommand(UTEXT('mod'));
      return sCommand;
   }

   bool ModuleGenerator::generate(const CommandLine& commandline)
   {
      // get the module name
      const CommandLineArgument* parg = commandline.getArgument(UTEXT("name"));
      if ( parg != nullptr )
      {
         String modname = parg->getValue();

         // for now we create the module in the crafter2d/src/mods folder

         FileSystem& system = FileSystem::getInstance();
         String path = UTEXT("../src/mods/mod_") + String(modname);
         if ( system.mkdir(path) )
         {
            // see what type of module to create
            parg = commandline.getArgument(UTEXT("type"));
            if ( parg != nullptr )
            {
               if ( parg->getValue() == UTEXT("content") )
               {
                  return generateContentModule(commandline, path, modname);
               }
               else
               {
                  std::cerr << parg->getValue().toUtf8() << " is not a supported module type.";
               }
            }
            else
            {
               std::cerr << "Missing required argument 'type'.";
            }
         }
      }
      else
      {
         std::cerr << "Missing required argument 'name'.";
      }

      return false;
   }

   bool ModuleGenerator::generateContentModule(const CommandLine& commandline, const String& path, const String& name)
   {
      FileSystem::getInstance().copyFile(UTEXT("../projects/templates/mod_dllmain.tpl"), File::concat(path, UTEXT("dllmain.cpp")));

      // figure out the required content for the template

      std::string baseHeader, baseClass, init;
      baseHeader = "modules/contentmodule.h";
      baseClass = "ContentModule";

      const CommandLineArgument* parg = commandline.getArgument(UTEXT("supports"));
      if ( parg != nullptr )
      {
         std::string exts = parg->getValue().toUtf8();
         init = "pmodule->setSupportedFiles(UTEXT(\"" + exts + "\"));";
      }

      // generate the file content
      ctemplate::TemplateDictionary dict("values");
      dict.SetValue("BASE_HEADER", baseHeader);
      dict.SetValue("BASE_CLASS", baseClass);
      dict.SetValue("NAME", name.toUtf8());
      dict.SetValue("INIT", init);

      std::string output;
      ctemplate::ExpandTemplate("../projects/templates/mod_content.tpl", ctemplate::DO_NOT_STRIP, &dict, &output);

      StdioFile file;
      String filename = File::concat(path, UTEXT("mod_") + String(name) + UTEXT(".cpp"));
      if ( file.open(filename, File::EWrite | File::EText) )
      {
         file.write(String(output));
         file.close();

         return true;
      }

      std::cerr << "Could not write to file " << filename.toUtf8();
      return false;
   }
}
