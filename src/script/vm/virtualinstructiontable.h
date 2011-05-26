
#ifndef VIRTUAL_INSTRUCTION_TABLE_H_
#define VIRTUAL_INSTRUCTION_TABLE_H_

#include <vector>

#include "virtualinstruction.h"

class LiteralTable;

class VirtualInstructionTable
{
   typedef std::vector<VirtualInstruction> Instructions;

public:
   VirtualInstructionTable();

   const VirtualInstruction& operator[](int index) const;
         VirtualInstruction& operator[](int index);

 // query
   int size() const;

 // adding
   void add(VirtualInstruction::Instruction instruction, int argument);
   void add(const VirtualInstruction& instruction);
   void add(const VirtualInstructionTable& instructions);

 // output
   std::string toString(const LiteralTable& table) const;

private:
   Instructions mInstructions;
};

#endif // VIRTUAL_INSTRUCTION_TABLE_H_
