
#ifndef CPU_H
#define CPU_H

namespace CodeGen
{
   class IRGenerator;
}

class CPU
{
public:
   CPU();

   virtual CodeGen::IRGenerator* createIRGenerator() = 0;
};

#endif // CPU_H
