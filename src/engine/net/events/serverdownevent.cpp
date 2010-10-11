
#include "serverdownevent.h"

#include "engine/net/netdefines.h"

IMPLEMENT_REPLICATABLE(ServerDownEventId, ServerDownEvent, NetEvent)

ServerDownEvent::ServerDownEvent():
   NetEvent(serverdownEvent)
{
}

void ServerDownEvent::pack(BitStream& stream) const
{
   NetEvent::pack(stream);
}

void ServerDownEvent::unpack(BitStream& stream)
{
   NetEvent::unpack(stream);
}
