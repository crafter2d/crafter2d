#ifndef MODULE_FILE_H
#define MODULE_FILE_H

class String;

namespace c2d
{
   struct ModuleInfo;
   struct Modules;

   class ModuleFile
   {
      typedef ModuleInfo*(*PGETMODULEINFO)();
      typedef void(*PFREEMODULEINFO)(ModuleInfo* pinfo);
      typedef Modules*(*PGETMODULES)();
      typedef void(*PFREEMODULES)(Modules* pmodules);

   public:
      explicit ModuleFile(const String& filename);
      ModuleFile(ModuleFile&& that);
      ModuleFile(const ModuleFile& that) = delete;
      ModuleFile& operator=(ModuleFile&& that);
      ModuleFile& operator=(const ModuleFile& that) = delete;
      ~ModuleFile();

    // query
      bool isValid() const {
         return mpHandle != nullptr;
      }

      const ModuleInfo& getInfo() const {
         return *mpInfo;
      }

      Modules& getModules() {
         return *mpModules;
      }

    // operations
      void load();

   private:
    // operations
      void move(ModuleFile& that);

    // members
      void*             mpHandle;
      PGETMODULEINFO    mpGetModuleInfo;
      PFREEMODULEINFO   mpFreeModuleInfo;
      PGETMODULES       mpGetModules;
      PFREEMODULES      mpFreeModules;
      ModuleInfo*       mpInfo;
      Modules*          mpModules;
   };
}

#endif // MODULE_FILE_H

