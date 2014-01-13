
#ifndef __YasScript_h__
#define __YasScript_h__

#include "core/script/script.h"

class VirtualObject;
class YasScriptManager;

class YasScript : public c2d::Script
{
public:
   YasScript(YasScriptManager& manager);

 // get/set
   void setThis(VirtualObject& object);

private:

 // data
   YasScriptManager& mManager;
   VirtualObject*    mpObject;
};

#endif // __YasScript_h__
