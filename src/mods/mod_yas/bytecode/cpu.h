
#ifndef CPU_H
#define CPU_H

#include "core/memory/memorypool.h"
#include "core/string/string.h"

#include "mod_yas/gc/garbagecollector.h"
#include "mod_yas/vm/virtualobject.h"
#include "mod_yas/script_base.h"

class VirtualArray;
class VirtualContext;
class VirtualClass;
class VirtualException;
class VirtualMachine;
class VirtualFunctionTableEntry;

namespace ByteCode
{
   class IRGenerator;
   
   class CPU
   {
   public:
      CPU(VirtualMachine& vm);
      virtual ~CPU();

    // query
      virtual IRGenerator* createIRGenerator() = 0;

    // operations
      void initialize(VirtualContext& context);

    // instantiation
      VirtualObject& instantiate(VirtualContext& context, const VirtualClass& klass, int constructor);
      VirtualArray*  instantiateArray();
      void           release(VirtualObject& object);

    // execution
      virtual VirtualValue execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry, int argc, VirtualValue* pargs) = 0;
      virtual void         executeStatic(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry) = 0;

    // garbage collection
      virtual void mark() = 0;

    // exception handling
      virtual String buildCallStack() const = 0;

   protected:
    // query
      GarbageCollector& getGC();
      VirtualMachine&   getVM();
      VirtualClass&     getArrayClass();
      VirtualClass&     getStringClass();

      bool isGarbageCollectionBlocked() const;
      void setGCBlock(bool blocked) {
         mBlockGC = blocked;
      }

      virtual void execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry) = 0;

    // exception handling
              void displayException(const VirtualException& exception);
              void throwException(VirtualContext& context, const String& exceptionname, const String& reason);
      virtual bool handleException(VirtualContext& context, VirtualObject& exception) = 0;

   private:

      VirtualMachine&            mVM;
      GarbageCollector           mGC;
      VirtualClass*              mpArrayClass;
      VirtualClass*              mpStringClass;
      MemoryPool<VirtualObject>  mObjectCache;
      bool                       mBlockGC;
   };
}

#endif // CPU_H
