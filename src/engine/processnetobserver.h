
#ifndef PROCESS_NET_OBSERVER_H
#define PROCESS_NET_OBSERVER_H

#include "core/defines.h"

#include "net/netobserver.h"

namespace c2d
{
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
}

#endif // PROCESS_NET_OBSERVER_H
