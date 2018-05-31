
#include "core/defines.h"

#include "querybodycomponentmessage.h"

QueryBodyComponentMessage::QueryBodyComponentMessage():
   ComponentMessage(ComponentInterface::eQueryBodyMsg, nullptr),
   mpBody(nullptr)
{
}

QueryBodyComponentMessage::~QueryBodyComponentMessage()
{
}

// - Get/set

bool QueryBodyComponentMessage::hasBody() const
{
   return mpBody != nullptr;
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
