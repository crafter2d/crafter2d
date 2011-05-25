
#ifndef VIRTUAL_MACHINE_OBJECT_OBSERVER_H_
#define VIRTUAL_MACHINE_OBJECT_OBSERVER_H_

#include "virtualobjectobserver.h"

class VirtualMachine;

class VirtualMachineObjectObserver : public VirtualObjectObserver
{
public:
   explicit VirtualMachineObjectObserver(VirtualMachine& machine);

 // notifications
   virtual void onCreated(VirtualObject& object);
   virtual void onDestroyed(VirtualObject& object);

private:

 // members
   VirtualMachine& mMachine;
};

#endif // VIRTUAL_MACHINE_OBJECT_OBSERVER_H_
