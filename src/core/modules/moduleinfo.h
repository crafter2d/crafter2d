
#ifndef MODULE_INFO_H
#define MODULE_INFO_H

#include "core/core_base.h"
#include <initializer_list>

namespace c2d
{
   class Uuid;

   class CORE_API ModuleInfo
   {
   public:
      ModuleInfo();
      ModuleInfo(ModuleInfo&& that) = delete;
      ModuleInfo(const ModuleInfo& that) = delete;
      ModuleInfo& operator=(ModuleInfo&& that) = delete;
      ModuleInfo& operator=(const ModuleInfo& that) = delete;

    // get/set
      int getVersion() const {
         return mVersion;
      }

      void setVersion(int version) {
         mVersion = version;
      }

      int getDependencyCount() {
         return mDepCount;
      }

      const Uuid* getDependencies() const {
         return mpDependencies;
      }

      void setDependencies(std::initializer_list<Uuid> list);

   private:

    // members
      Uuid* mpDependencies;
      int   mDepCount;
      int   mVersion;
   };
}

#endif // MODULE_INFO_H

