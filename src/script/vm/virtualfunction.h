
#ifndef VIRTUAL_FUNCTION_H
#define VIRTUAL_FUNCTION_H

#include "virtualinstructiontable.h"

class VirtualFunction
{
public:
   VirtualFunction();

   void addInstruction(const VirtualInstruction& instruction);

private:

 // operations
   VirtualInstructionTable mInstructions;
};

#endif // VIRTUAL_FUNCTION_H
