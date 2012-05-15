
#include "serverdownevent.h"

#include "engine/net/netdefines.h"

IMPLEMENT_REPLICATABLE(ServerDownEventId, ServerDownEvent, NetEvent)

ServerDownEvent::ServerDownEvent():
   NetEvent(serverdownEvent)
{
}
