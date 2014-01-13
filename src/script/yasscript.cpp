
#include "yasscript.h"

#include "core/defines.h"

YasScript::YasScript(YasScriptManager& manager) :
   Script(),
   mManager(manager),
   mpObject(NULL)
{

}

// - Get/set

void YasScript::setThis(VirtualObject& object)
{
   mpObject = &object;
}
