
#ifndef VIRTUAL_FUNCTION_H
#define VIRTUAL_FUNCTION_H

#include "virtualinstructiontable.h"

class VirtualFunction
{
public:
   VirtualFunction();

   void addInstruction(const VirtualInstruction& instruction);
   void prepare();

private:

 // operations
   void replaceLabels();
   void removeNops();

   int  findLabel(int label) const;

   VirtualInstructionTable mInstructions;
};

#endif // VIRTUAL_FUNCTION_H
