

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include "core/containers/hashmap.h"

#include "script/vm/virtualobjectreference.h"

class VirtualMachine;

class GarbageCollector
{
public:
   GarbageCollector();
   ~GarbageCollector();

 // operations
   void collect(VirtualObjectReference& object);
   void gc(VirtualMachine& vm);

private:
   typedef HashMap<void*, VirtualObjectReference> Objects;

   Objects mObjects;
};

#endif // GARBAGE_COLLECTOR_H
