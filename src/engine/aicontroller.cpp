
#include "aicontroller.h"

#include "core/smartptr/autoptr.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "process.h"

AIController::AIController(Process& process):
   Controller(),
   mProcess(process),
   mpScript(NULL)
{
}

// - Operations

void AIController::performAction(Actor& actor)
{
   AutoPtr<Script> script = mProcess.getScriptManager().loadNative("Actor", &actor, false);
   script->run("updateAI");
}
