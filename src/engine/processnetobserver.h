
#ifndef PROCESS_NET_OBSERVER_H
#define PROCESS_NET_OBSERVER_H

#include "net/netobserver.h"

class Process;

class ProcessNetObserver : public NetObserver
{
public:
   explicit ProcessNetObserver(Process& process);

 // implementation
   virtual void onEvent(int clientid, const NetEvent& event);

private:
   Process& mProcess;
};

#endif // PROCESS_NET_OBSERVER_H
