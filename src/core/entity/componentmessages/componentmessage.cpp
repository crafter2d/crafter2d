
#include "componentmessage.h"

ComponentMessage::ComponentMessage(ComponentInterface::ComponentMessageType messagetype, void* pdata):
	mMessageType(messagetype),
	mpData(pdata)
{
}

ComponentMessage::~ComponentMessage()
{
}
