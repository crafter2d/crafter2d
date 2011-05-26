
#include "virtualmachineobjectobserver.h"

#include "virtualmachine.h"
#include "virtualobject.h"

VirtualMachineObjectObserver::VirtualMachineObjectObserver(VirtualMachine& machine):
   VirtualObjectObserver(),
   mMachine(machine)
{
}

// notifications
   
void VirtualMachineObjectObserver::onCreated(VirtualObject& object)
{
   //mMachine.insertNative(object);
}

void VirtualMachineObjectObserver::onDestroyed(VirtualObject& object)
{
   //mMachine.deleteNative(object);
}
