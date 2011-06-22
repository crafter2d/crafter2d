
#include "scriptmouseeventdispatcher.h"

#include "core/defines.h"

#include "engine/script/scriptmanager.h"
#include "engine/script/script.h"

ScriptMouseEventDispatcher::ScriptMouseEventDispatcher():
   MouseEventDispatcher(),
   mpScript(NULL)
{
}

ScriptMouseEventDispatcher::~ScriptMouseEventDispatcher()
{
   delete mpScript;
   mpScript = NULL;
}

// - Initialization

bool ScriptMouseEventDispatcher::create(ScriptManager& manager, const VirtualObjectReference& self)
{
   mpScript = manager.loadClass("MouseEventDispatcher");
   if ( mpScript == NULL )
   {
      return false;
   }

   mpScript->setThis(self);

   return true;
}

// - Dispatching

void ScriptMouseEventDispatcher::dispatch(const MouseEvent& event)
{
}
