

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include "core/containers/hashmap.h"

class Variant;
class VirtualObject;
class VirtualMachine;

class GarbageCollector
{
public:
   GarbageCollector();
   ~GarbageCollector();

 // operations
   void gc(VirtualMachine& vm);

private:
   typedef HashMap<void*, VirtualObject*> Objects;

   void phaseMark(VirtualMachine& vm);
   void phaseCollect(VirtualMachine& vm);
      
   Objects mObjects;
};

#endif // GARBAGE_COLLECTOR_H
