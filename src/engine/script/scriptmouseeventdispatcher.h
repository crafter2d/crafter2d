
#ifndef SCRIPT_MOUSE_EVENT_DISPATCHER_H_
#define SCRIPT_MOUSE_EVENT_DISPATCHER_H_

#include "core/input/mouseeventdispatcher.h"

class Script;
class ScriptManager;
class VirtualObjectReference;

class ScriptMouseEventDispatcher : public MouseEventDispatcher
{
public:
   ScriptMouseEventDispatcher();
   virtual ~ScriptMouseEventDispatcher();

 // initialization
   bool create(ScriptManager& manager, const VirtualObjectReference& self);

 // overloads
   virtual void dispatch(const MouseEvent& event);

private:

 // members
   Script*  mpScript;
};

#endif // SCRIPT_MOUSE_DISPATCHER_H_
