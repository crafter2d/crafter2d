
#include "classgenerator.h"

#include <io.h>
#include <iostream>
#include <ctemplate/template.h>

#include "core/string/string.h"
#include "core/commandline/commandline.h"
#include "core/commandline/commandlineargument.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/stdiofile.h"

bool ClassGenerator::generate(CommandLine& commandline)
{
   const CommandLineArgument& nameargument = commandline.getArgument(UTEXT("name"));
   const CommandLineArgument* pbaseargument = commandline.findArgument(UTEXT("base"));
   const CommandLineArgument& pathargument = commandline.getArgument(UTEXT("path"));

   String package;
   String name = nameargument.getValue();
   int pos = name.lastIndexOf(L'.');
   if ( pos == -1 )
   {
      const CommandLineArgument& packageargument = commandline.getArgument(UTEXT("package"));
      package = packageargument.getValue();
   }
   else
   {
      package = name.left(pos - 1);
      name = name.right(pos + 1);
   }

   ctemplate::TemplateDictionary dict("values");
   dict.SetValue("NAME", name.toUtf8());
   dict.SetValue("PACKAGE", package.toUtf8());
   if ( pbaseargument != NULL )
   {
      dict.SetValue("BASE", "extends " + pbaseargument->getValue().toUtf8());
   }

   std::string expanded;
   std::string templ = getTemplateFile(UTEXT("class")).toUtf8();
   if ( !ctemplate::ExpandTemplate(templ, ctemplate::DO_NOT_STRIP, &dict, &expanded) )
   {
      std::cerr << "Could not expand the class template.";
      return false;
   }

   String path = package;
   path.replace('.', FileSystem::getInstance().getNativeSeparator());
   path = File::concat(File::concat(pathargument.getValue(), UTEXT("scripts")), File::concat(path, name + UTEXT(".as")));

   return writeFile(path, String(expanded));
}

bool ClassGenerator::writeFile(const String& filename, const String& output)
{
   StdioFile file;
   if ( file.open(filename, File::EWrite | File::EText) )
   {
      file.write(output);
      return true;
   }

   std::cerr << "Could not write to file " << filename.toUtf8() << std::endl;

   return false;
}