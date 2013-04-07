
#include "virtualfunction.h"

#include "core/defines.h"

VirtualFunction::VirtualFunction():
   mInstructions()
{
}

void VirtualFunction::addInstruction(const VirtualInstruction& instruction)
{
   mInstructions.add(instruction);
}

void VirtualFunction::prepare()
{
   replaceLabels();
   removeNops();
}

