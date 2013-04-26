
#ifndef BYTECODE_INSTRUCTION_H
#define BYTECODE_INSTRUCTION_H

class ASTType;

#define MAKE_INST(opcode,arg)((opcode << 24) | arg)
#define INST_OPCODE(inst) ((inst >> 24) & 0xFF)
#define INST_ARG(inst) (inst & 0xFFFFFF)

#define INSERT(opc,a)                                          \
   do {                                                        \
      ByteCode::Instruction* ins = new ByteCode::Instruction();\
      ins->inst = MAKE_INST(opc,a);                            \
      pblock->addInstruction(ins);                             \
   } while (false);

namespace ByteCode
{
   class Instruction
   {
   public:
      Instruction();

      Instruction*   next;
      int            inst;
   };
}

#endif // BYTECODE_INSTRUCTION_H
