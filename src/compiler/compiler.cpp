// compiler.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <tinyxml.h>

#include "core/streams/bufferedstream.h"
#include "core/content/contentwriter.h"
#include "core/content/contentmanager.h"
#include "core/string/string.h"
#include "core/modules/contentmodule.h"
#include "core/modules/modulemanager.h"
#include "core/vfs/stdiofile.h"

using namespace c2d;

void loadModules(const String& srcfile, const String& dstFile)
{
   ModuleManager mgr;
   mgr.initialize();
   ModuleCollection mods = mgr.filter(ModuleKind::eContentModule);

   int index = srcfile.lastIndexOf(L'.');
   String extension = srcfile.subStr(index + 1, srcfile.length() - index - 1);

   if ( extension == UTEXT("xml") )
   {
      TiXmlDocument doc(srcfile.toUtf8().c_str());
      if ( !doc.LoadFile() )
      {
         // invalid file format
         return;
      }

      TiXmlElement* proot = doc.FirstChildElement();
      if ( proot != NULL )
      {
         extension = String::fromUtf8(proot->Value());
      }
   }

   ModuleCollectionIterator it = mods.getIterator();
   for ( ; it.isValid(); ++it )
   {
      ContentModule& cmod = static_cast<ContentModule&>(*it);
      if ( cmod.supports(extension) )
      {
         BufferedStream stream;
         cmod.getUuid().write(stream);

         try
         {
            ContentWriter& writer = cmod.getWriter();
            if ( writer.write(stream, srcfile) )
            {
               StdioFile file;
               file.open(dstFile, File::EBinary | File::EWrite);
               file.write(stream.getData(), stream.getDataSize());
               file.close();

               return;
            }
         }
         catch ( std::exception* )
         {
         }
         break;
      }
   }
}

int _tmain(int argc, _TCHAR* argv[])
{
   if ( argc < 3 )
   {
      printf("Use: compiler {sourcefile} {destfile}");
      return -1;
   }

   String srcfile = String::fromUtf8(argv[1]);
   String dstfile = String::fromUtf8(argv[2]);

   loadModules(srcfile, dstfile);

	return 0;
}

