// compiler.cpp : Defines the entry point for the console application.
//

#include "compiler.h"

#include <algorithm>

#include "core/streams/bufferedstream.h"
#include "core/content/contentwriter.h"
#include "core/content/contentmanager.h"
#include "core/string/string.h"
#include "core/string/stringinterface.h"
#include "core/modules/contentmodule.h"
#include "core/modules/modulemanager.h"
#include "core/vfs/stdiofile.h"
#include "core/vfs/filesystem.h"
#include "core/system/exception.h"

#include "xml/xml/xmltools.h"

using namespace c2d;

static const int SUCCESS               = 0;
static const int ERR_FILEINUSE         = -1;
static const int ERR_NOCOMPILER        = -2;
static const int ERR_EXCEPTION         = -3;
static const int ERR_INVALIDFILEFORMAT = -4;
static const int ERR_MODULEMANAGER     = -5;
static const int ERR_UNKNOWN           = -6;

int main(int argc, char *argv[])
{
   c2d::compiler::Compiler compiler(argc, argv);
   return compiler.exec();
}

namespace c2d::compiler
{
   Compiler::Compiler(int argc, char* pargv[]) :
      mCommandLine(argc, pargv),
      mFiles(),
      mSource(),
      mDest()
   {
      init();
   }

   void Compiler::init()
   {
      if ( mCommandLine.size() < 2 )
      {
         throw new std::runtime_error("Use: compiler {sourcefile} {destfile}");
      }
      
      mSource = mCommandLine[0].getName();
      mDest = mCommandLine[1].getName();

      FileSystem::getInstance().find(mSource, mFiles, true);

      if ( mSource[mSource.length() - 1] == L'*' )
      {
         mSource = mSource.left(mSource.length() - 1);

         std::vector<String> filters;
         filters.push_back(UTEXT("*.craft"));
         filters.push_back(mSource + UTEXT("images"));
         filters.push_back(mSource + UTEXT("build"));
         filters.push_back(mSource + UTEXT("scripts"));
         
         auto it = std::remove_if(mFiles.begin(), mFiles.end(), [filters](String& file) {
            for ( auto& filter : filters )
            {
               size_t pos;
               if ( filter[0] == L'*' )
               {
                  String realfilter = filter.right(1);
                  pos = file.indexOf(realfilter);
               }
               else
                  pos = file.indexOf(filter);

               if ( pos != String::npos )
               {
                  return true;
               }
            }
            return false;
         });
         mFiles.erase(it, mFiles.end());
      }
   }

   int Compiler::exec()
   {
      int ret = compile();
      if ( ret == SUCCESS )
      {
         printf("Compiled successfully!");
      }
      else
      {
         printf("Failed to compile");
      }
      return ret;
   }

   int Compiler::compile()
   {
      ModuleManager mgr;
      if ( !mgr.initialize() )
         return ERR_MODULEMANAGER;

      for ( const auto& file : mFiles )
      {
         try
         {
            String extension = determineExtension(file);
            ContentModule& cmod = resolveModule(mgr, extension);

            BufferedStream stream;
            cmod.getUuid().write(stream);

            ContentWriter& writer = cmod.getWriter();
            if ( writer.write(stream, file) )
            {
               // src = ../demo/world/world.j
               // dst = ../joop/build/ -> world/world.c2d

               String filename = file.right(mSource.length());
               auto pos = filename.lastIndexOf(L'.');
               ASSERT(pos != String::npos);
               filename = File::concat(mDest, filename.left(pos));
               save(filename, stream);
            }
            else
            {
               throw new std::runtime_error("Could not process file");
            }
         }
         catch ( std::exception* pex )
         {
            printf("Error (%s) - %s\n", file.toUtf8().c_str(), pex->what());
         }
      }

      return SUCCESS;
   }

   String Compiler::determineExtension(const String& filename)
   {
      std::size_t index = filename.lastIndexOf(L'.');
      String extension = filename.subStr(index + 1, filename.length() - index - 1);

      if ( extension == UTEXT("xml") )
      {
         extension = c2d::xml::tools::rootElement(filename);
      }
      return extension;
   }

   void Compiler::save(const String& filename, const DataStream& stream)
   {
      String outfile = filename;
      size_t pos = outfile.lastIndexOf(L'.');
      // if there is no dot or if there is a dir separator after it we append the extention
      if ( pos < 0 || outfile.indexOf(FileSystem::getNativeSeparator(), pos) > pos )
      {
         outfile += UTEXT(".c2d");
      }

      FileSystem::getInstance().mkpath(outfile);

      StdioFile file;
      if ( !file.open(outfile, File::EBinary | File::EWrite) )
      {
         throw new std::runtime_error("Could not open file " + outfile.toUtf8());
      }

      file.write(stream.getData(), stream.getDataSize());
      file.close();
   }

   ContentModule& Compiler::resolveModule(ModuleManager& mgr, const String& extension)
   {
      ModuleCollection mods = mgr.filter(ModuleKind::eContentModule);
      for ( auto& pair : mods )
      {
         ContentModule& cmod = static_cast<ContentModule&>(*pair.second);
         if ( cmod.supports(extension) )
         {
            return cmod;
         }
      }

      throw new std::runtime_error("There is no compiler found for this file");
   }
}
