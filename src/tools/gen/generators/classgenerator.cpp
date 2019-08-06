
#include "classgenerator.h"

#ifdef WIN32
#include <io.h>
#endif

#include <iostream>
#include <algorithm>
#include <ctemplate/template.h>

#include "core/string/string.h"
#include "core/commandline/commandline.h"
#include "core/commandline/commandlineargument.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/stdiofile.h"

namespace c2d::gen
{
   GENERATOR_IMPL(ClassGenerator);

   template<class T>
   int inline findAndReplace(T& source, const T& find, const T& replace)
   {
      int num = 0;
      std::size_t fLen = find.size();
      std::size_t rLen = replace.size();
      for ( std::size_t pos = 0; (pos = source.find(find, pos)) != T::npos; pos += rLen )
      {
         num++;
         source.replace(pos, fLen, replace);
      }
      return num;
   }

   UChar ClassGenerator::getBriefCommand() const
   {
      return L'c';
   }

   const String& ClassGenerator::getCommand() const
   {
      static const String sCommand(UTEXT("class"));
      return sCommand;
   }

   bool ClassGenerator::generate(CommandLine& commandline)
   {
      const CommandLineArgument* pnameargument = commandline.getArgument(UTEXT("name"));
      const CommandLineArgument* pbaseargument = commandline.getArgument(UTEXT("base"));
      const CommandLineArgument* ppathargument = commandline.getArgument(UTEXT("path"));

      if ( pnameargument == nullptr || ppathargument == nullptr )
      {
         return false;
      }

      String package;
      String name = pnameargument->getValue();
      std::size_t pos = name.lastIndexOf(L'.');
      if ( pos == String::npos )
      {
         const CommandLineArgument* ppackageargument = commandline.getArgument(UTEXT("package"));
         if ( ppackageargument == nullptr )
         {
            return false;
         }

         package = ppackageargument->getValue();
      }
      else
      {
         package = name.left(pos - 1);
         name = name.right(pos + 1);
      }

      ctemplate::TemplateDictionary dict("values");
      dict.SetValue("NAME", name.toUtf8());
      dict.SetValue("PACKAGE", package.toUtf8());
      if ( pbaseargument != nullptr )
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

      // needed so that Crafter Workshop does not show invalid empty lines
      findAndReplace(expanded, std::string("\r\n"), std::string("\n"));

      String path = package;
      path.replace('.', FileSystem::getInstance().getNativeSeparator());
      path = File::concat(File::concat(ppathargument->getValue(), UTEXT("scripts")), File::concat(path, name + UTEXT(".as")));

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
}
