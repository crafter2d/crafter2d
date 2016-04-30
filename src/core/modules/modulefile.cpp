#include "modulefile.h"

#include "core/string/string.h"
#include "core/system/platform.h"

c2d::ModuleFile::ModuleFile(const String & filename):
   mpHandle(nullptr),
   mpGetModuleInfo(nullptr),
   mpFreeModuleInfo(nullptr),
   mpGetModules(nullptr),
   mpFreeModules(nullptr),
   mpInfo(nullptr),
   mpModules(nullptr)
{
   Platform& platform = Platform::getInstance();

   mpHandle = platform.loadModule(filename);
   if ( mpHandle != nullptr )
   {
      mpGetModuleInfo = (PGETMODULEINFO) platform.getFunctionAddress(mpHandle, UTEXT("getModuleInfo"));
      mpFreeModuleInfo = (PFREEMODULEINFO) platform.getFunctionAddress(mpHandle, UTEXT("freeModuleInfo"));
      mpGetModules = (PGETMODULES) platform.getFunctionAddress(mpHandle, UTEXT("getModules"));
      mpFreeModules = (PFREEMODULES) platform.getFunctionAddress(mpHandle, UTEXT("freeModules"));

      if ( mpGetModuleInfo != nullptr )
      {
         mpInfo = (*mpGetModuleInfo)();
      }
   }
}

c2d::ModuleFile::ModuleFile(ModuleFile&& that) :
   mpHandle(nullptr),
   mpGetModuleInfo(nullptr),
   mpFreeModuleInfo(nullptr),
   mpGetModules(nullptr),
   mpFreeModules(nullptr),
   mpInfo(nullptr),
   mpModules(nullptr)
{
   move(that);
}

c2d::ModuleFile& c2d::ModuleFile::operator=(ModuleFile&& that)
{
   move(that);
   return *this;
}

c2d::ModuleFile::~ModuleFile()
{
   if ( mpInfo != nullptr )
   {
      (*mpFreeModuleInfo)(mpInfo);
   }
   if ( mpModules != nullptr )
   {
      (*mpFreeModules)(mpModules);
   }
   Platform::getInstance().freeModule(mpHandle);
}

void c2d::ModuleFile::move(ModuleFile& that)
{
   mpHandle = that.mpHandle;
   mpGetModuleInfo = that.mpGetModuleInfo;
   mpFreeModuleInfo = that.mpFreeModuleInfo;
   mpGetModules = that.mpGetModules;
   mpFreeModules = that.mpFreeModules;
   mpInfo = that.mpInfo;
   mpModules = that.mpModules;

   that.mpHandle = nullptr;
   that.mpInfo = nullptr;
   that.mpModules = nullptr;
}

void c2d::ModuleFile::load()
{
   mpModules = (*mpGetModules)();
}