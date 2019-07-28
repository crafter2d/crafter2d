// compiler.cpp : Defines the entry point for the console application.
//

#include <tinyxml.h>

#include "core/streams/bufferedstream.h"
#include "core/content/contentwriter.h"
#include "core/content/contentmanager.h"
#include "core/string/string.h"
#include "core/modules/contentmodule.h"
#include "core/modules/modulemanager.h"
#include "core/vfs/stdiofile.h"
#include "core/system/exception.h"
#include "core/system/platform.h"

using namespace c2d;

static const int SUCCESS               = 0;
static const int ERR_FILEINUSE         = -1;
static const int ERR_NOCOMPILER        = -2;
static const int ERR_EXCEPTION         = -3;
static const int ERR_INVALIDFILEFORMAT = -4;

String determineExtension(const String& filename)
{
   std::size_t index = filename.lastIndexOf(L'.');
   String extension = filename.subStr(index + 1, filename.length() - index - 1);

   if ( extension == UTEXT("xml") )
   {
      TiXmlDocument doc(filename.toUtf8().c_str());
      if ( !doc.LoadFile() )
      {
         throw new std::exception("File does not contain proper XML.");
      }

      TiXmlElement* proot = doc.FirstChildElement();
      if ( proot != nullptr )
      {
         extension = String::fromUtf8(proot->Value());
      }
   }
   return extension;
}

ContentModule& resolveModule(ModuleManager& mgr, const String& extension)
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

   throw new std::exception("There is no compiler found for this file");
}

void save(const String& filename, const DataStream& stream)
{
   String outfile = filename;
   int pos = outfile.lastIndexOf(L'.');
   // if there is no dot or if there is a dir separator after it we append the extention
   if ( pos < 0 || outfile.indexOf(Platform::getInstance().preferedSlash(), pos) > pos )
   {
      outfile += UTEXT(".c2d");
   }

   StdioFile file;
   if ( !file.open(outfile, File::EBinary | File::EWrite) )
   {
      throw new Exception(UTEXT("Could not open file ") + outfile);
   }

   file.write(stream.getData(), stream.getDataSize());
   file.close();
}

int compile(const String& srcfile, const String& dstFile)
{
   try
   {
      ModuleManager mgr;
      mgr.initialize();      
            
      BufferedStream stream;

      String extension = determineExtension(srcfile);
      ContentModule& cmod = resolveModule(mgr, extension);
      cmod.getUuid().write(stream);

      ContentWriter& writer = cmod.getWriter();
      if ( writer.write(stream, srcfile) )
      {
         save(dstFile, stream);
      }
   }
   catch ( c2d::Exception* pex )
   {
      std::string err = pex->getReason().toUtf8();
      printf("Failed to compile the file: %s", err.c_str());
      return ERR_EXCEPTION;
   }
   catch ( std::exception* pex )
   {
      printf("Failed to compile the file: %s", pex->what());
      return ERR_EXCEPTION;
   }
   
   return SUCCESS;
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

