// compiler.cpp : Defines the entry point for the console application.
//

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>

#include "core/content/contentmodule.h"
#include "core/content/contentwriter.h"
#include "core/content/contentmanager.h"
#include "core/string/string.h"
#include "core/modules/modulemanager.h"

void loadModules(const String& srcfile, const String& dstFile)
{
   ModuleCollection cmods(Module::eContent);
   ModuleManager mgr;
   mgr.initialize();
   mgr.getModules(cmods);

   int index = srcfile.lastIndexOf(L'.');
   String extension = srcfile.subStr(index + 1, srcfile.length() - index - 1);

   for ( int index = 0; index < cmods.size(); ++index )
   {
      ContentModule& cmod = static_cast<ContentModule&>(cmods[index]);
      if ( cmod.supports(extension) )
      {
         cmod.write(srcfile, dstFile);
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

