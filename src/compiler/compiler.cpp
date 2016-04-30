// compiler.cpp : Defines the entry point for the console application.
//

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#endif

#include <tinyxml.h>

#include "core/streams/bufferedstream.h"
#include "core/content/contentwriter.h"
#include "core/content/contentmanager.h"
#include "core/string/string.h"
#include "core/modules/contentmodule.h"
#include "core/modules/modulemanager.h"
#include "core/vfs/stdiofile.h"
#include "core/system/exception.h"

using namespace c2d;

static const int SUCCESS               = 0;
static const int ERR_FILEINUSE         = -1;
static const int ERR_NOCOMPILER        = -2;
static const int ERR_EXCEPTION         = -3;
static const int ERR_INVALIDFILEFORMAT = -4;

int compile(const String& srcfile, const String& dstFile)
{
   try
   {
      ModuleManager mgr;
      mgr.initialize();
      ModuleCollection mods = mgr.filter(ModuleKind::eContentModule);

      std::size_t index = srcfile.lastIndexOf(L'.');
      String extension = srcfile.subStr(index + 1, srcfile.length() - index - 1);

      if ( extension == UTEXT("xml") )
      {
         TiXmlDocument doc(srcfile.toUtf8().c_str());
         if ( !doc.LoadFile() )
         {
            // invalid file format
            return ERR_INVALIDFILEFORMAT;
         }

         TiXmlElement* proot = doc.FirstChildElement();
         if ( proot != NULL )
         {
            extension = String::fromUtf8(proot->Value());
         }
      }

      for ( auto& pair : mods )
      {
         ContentModule& cmod = static_cast<ContentModule&>(*pair.second);
         if ( cmod.supports(extension) )
         {
            BufferedStream stream;
            cmod.getUuid().write(stream);

            ContentWriter& writer = cmod.getWriter();
            if ( writer.write(stream, srcfile) )
            {
               StdioFile file;
               if ( file.open(dstFile, File::EBinary | File::EWrite) )
               {
                  file.write(stream.getData(), stream.getDataSize());
                  file.close();
                  return SUCCESS;
               }
               else
               {
                  // error 
                  return ERR_FILEINUSE;
               }
            }
         }
      }
   }
   catch ( c2d::Exception& e )
   {
      std::string err = e.getReason().toUtf8();
      printf("Failed to compile the file: %s", err.c_str());
      return ERR_EXCEPTION;
   }
   catch ( std::exception& ex )
   {
      printf("Failed to compile the file: %s", ex.what());
      return ERR_EXCEPTION;
   }
   
   return ERR_NOCOMPILER;
}

int main(int argc, char *argv[])
{
   if ( argc < 3 )
   {
      printf("Use: compiler {sourcefile} {destfile}");
      return -1;
   }

   String srcfile = String::fromUtf8(argv[1]);
   String dstfile = String::fromUtf8(argv[2]);
   
   int ret = compile(srcfile, dstfile);
   if ( ret == SUCCESS )
   {
      printf("Compiled successfully!");
   }
   else
   {
      printf("Failed to compile");
   }
}

