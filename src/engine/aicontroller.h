
#ifndef AI_CONTROLLER_H
#define AI_CONTROLLER_H

#include "controller.h"

class Process;
class Script;

class AIController : public Controller
{
public:
   explicit AIController(Process& process);

 // operations
   virtual void performAction(Actor& actor);

private:
   Process& mProcess;
   Script*  mpScript;
};

#endif // AI_CONTROLLER_H
