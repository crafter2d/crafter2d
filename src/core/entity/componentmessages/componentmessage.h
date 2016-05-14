
#ifndef COMPONENT_MESSAGE_H
#define COMPONENT_MESSAGE_H

#include "core/defines.h"
#include "core/core_base.h"

#include "core/entity/components/componentinterface.h"

class CORE_API ComponentMessage
{
public:
	ComponentMessage(ComponentInterface::ComponentMessageType messagetype, void* pdata = NULL);
   virtual ~ComponentMessage();

 // get/set
	ComponentInterface::ComponentMessageType getMessageType() const;
	void* getData() const;

private:

 // data members
	ComponentInterface::ComponentMessageType mMessageType;
	void* mpData;
};

#endif // COMPONENT_MESSAGE_H
