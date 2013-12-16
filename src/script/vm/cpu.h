
#ifndef CPU_H
#define CPU_H

#include "core/memory/memorypool.h"
#include "core/string/string.h"

#include "script/gc/garbagecollector.h"
#include "script/script_base.h"
#include "script/vm/virtualobject.h"

namespace ByteCode
{
   class IRGenerator;
   class Program;
}

class Variant;
class VirtualArray;
class VirtualContext;
class VirtualClass;
class VirtualException;
class VirtualMachine;
class VirtualFunctionTableEntry;

class SCRIPT_API CPU
{
public:
   CPU(VirtualMachine& vm);
   virtual ~CPU();

 // get/set
   ByteCode::Program& getProgram();

 // query
   virtual ByteCode::IRGenerator* createIRGenerator() = 0;

 // operations
   void initialize(VirtualContext& context);

 // instantiation
   VirtualObject& instantiate(VirtualContext& context, const VirtualClass& klass, int constructor);
   VirtualArray*  instantiateArray();
   void           release(VirtualObject& object);

 // execution
   virtual Variant execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry, int argc, Variant* pargs) = 0;
   virtual Variant execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry) = 0;
   virtual void    execute(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry) = 0;

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
      
 // exception handling
           void displayException(const VirtualException& exception);
           void throwException(VirtualContext& context, const String& exceptionname, const String& reason);
   virtual bool handleException(VirtualContext& context, VirtualObject& exception) = 0;

private:

   VirtualMachine&            mVM;
   GarbageCollector           mGC;
   ByteCode::Program*         mpProgram;
   VirtualClass*              mpArrayClass;
   VirtualClass*              mpStringClass;
   MemoryPool<VirtualObject>  mObjectCache;
   bool                       mBlockGC;
};

#endif // CPU_H
