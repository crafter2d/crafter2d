
#include "serverdownevent.h"

#include "engine/net/netdefines.h"

IMPLEMENT_REPLICATABLE(ServerDownEventId, ServerDownEvent, NetEvent)

ServerDownEvent::ServerDownEvent():
   NetEvent(serverdownEvent)
{
}

// - Streaming

void ServerDownEvent::doPack(BitStream& stream) const
{
   NetEvent::doPack(stream);
}

void ServerDownEvent::doUnpack(BitStream& stream, int dirtyflag)
{
   NetEvent::doUnpack(stream, dirtyflag);
}
