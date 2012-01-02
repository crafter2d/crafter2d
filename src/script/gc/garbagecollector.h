

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include <list>

#include "script/vm/virtualobjectreference.h"

class VirtualMachine;

class GarbageCollector
{
public:
   GarbageCollector();

   void collect(VirtualObjectReference& object);
   void gc(VirtualMachine& vm);

private:
   typedef std::list<VirtualObjectReference> Objects;

   Objects mObjects;
};

#endif // GARBAGE_COLLECTOR_H
