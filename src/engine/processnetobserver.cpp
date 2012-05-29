
#include "processnetobserver.h"

#include "process.h"

ProcessNetObserver::ProcessNetObserver(Process& process):
   NetObserver(),
   mProcess(process)
{
}

// - Implementation

void ProcessNetObserver::onEvent(int clientid, const NetEvent& event)
{
   mProcess.onNetEvent(clientid, event);
}
