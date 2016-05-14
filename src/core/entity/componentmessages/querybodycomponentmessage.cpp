
#include "core/defines.h"

#include "querybodycomponentmessage.h"

QueryBodyComponentMessage::QueryBodyComponentMessage():
   ComponentMessage(ComponentInterface::eQueryBodyMsg, NULL),
   mpBody(NULL)
{
}

QueryBodyComponentMessage::~QueryBodyComponentMessage()
{
}

// - Get/set

bool QueryBodyComponentMessage::hasBody() const
{
   return mpBody != NULL;
}

Body& QueryBodyComponentMessage::getBody()
{
   ASSERT_PTR(mpBody);
   return *mpBody;
}

void QueryBodyComponentMessage::setBody(Body& body)
{
   mpBody = &body;
}
