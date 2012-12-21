
#ifndef COMPONENT_MESSAGE_H
#define COMPONENT_MESSAGE_H

#include "core/defines.h"

#include "componentinterface.h"

class ComponentMessage
{
public:
	ComponentMessage(ComponentInterface::ComponentMessageType messagetype, void* pdata = NULL);

 // get/set
	ComponentInterface::ComponentMessageType getMessageType() const;
	void* getData() const;

private:
	ComponentInterface::ComponentMessageType mMessageType;
	void* mpData;
};

#endif // COMPONENT_MESSAGE_H
