
#ifndef CPU_H
#define CPU_H

#include "script/script_base.h"

namespace ByteCode
{
   class IRGenerator;
}

class SCRIPT_API CPU
{
public:
   CPU();

   virtual ByteCode::IRGenerator* createIRGenerator() = 0;
};

#endif // CPU_H
