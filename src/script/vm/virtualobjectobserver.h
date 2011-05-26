
#ifndef VIRTUAL_OBJECT_OBSERVER_H_
#define VIRTUAL_OBJECT_OBSERVER_H_

class VirtualObject;

class VirtualObjectObserver
{
public:
   
   virtual void onCreated(VirtualObject& object) = 0;
   virtual void onDestroyed(VirtualObject& object) = 0;
};

#endif // VIRTUAL_OBJECT_OBSERVER_H_
