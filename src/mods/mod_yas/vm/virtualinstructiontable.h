
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
   bool isEmpty() const;
   int size() const;

 // adding
   void add(VirtualInstruction::Opcode opcode, int argument);
   void add(const VirtualInstruction& instruction);
   void add(const VirtualInstructionTable& instructions);
   void remove(int index);

 // output
   std::string toString(const LiteralTable& table) const;

private:
   Instructions mInstructions;
};

#endif // VIRTUAL_INSTRUCTION_TABLE_H_
