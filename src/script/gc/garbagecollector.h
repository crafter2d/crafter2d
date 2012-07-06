

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

class VirtualMachine;

class GarbageCollector
{
public:
   GarbageCollector();
   ~GarbageCollector();

 // operations
   void gc(VirtualMachine& vm);

private:
 // phases
   void phaseMark(VirtualMachine& vm);
   void phaseCollect(VirtualMachine& vm);
};

#endif // GARBAGE_COLLECTOR_H
