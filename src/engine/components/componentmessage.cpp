
#include "componentmessage.h"

ComponentMessage::ComponentMessage(ComponentInterface::ComponentMessageType messagetype, void* pdata):
	mMessageType(messagetype),
	mpData(pdata)
{
}

// - Get/set

ComponentInterface::ComponentMessageType ComponentMessage::getMessageType() const
{
	return mMessageType;
}

void* ComponentMessage::getData() const
{
	return mpData;
}
