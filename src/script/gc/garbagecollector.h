

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include "core/containers/list.h"

class Collectable;
class VirtualMachine;

class GarbageCollector
{
public:
   GarbageCollector();
   ~GarbageCollector();

 // operations
   void collect(Collectable* pcollectable);

   void gc(VirtualMachine& vm);

private:
   typedef List<Collectable*> Collectables;

 // phases
   void phaseMark(VirtualMachine& vm);
   void phaseCollect(VirtualMachine& vm);

   Collectables mCollectables;
};

#endif // GARBAGE_COLLECTOR_H
