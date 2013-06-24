
#ifndef JIT_METHOD_BUILDER_H
#define JIT_METHOD_BUILDER_H

class JITFunction;
class JITValue;

struct OpcodeDesc;

class JITMethodBuilder
{
public:
   explicit JITMethodBuilder(JITFunction& function);

 // builders
   JITValue* addMul(JITValue* value1, JITValue* value2);

private:
   JITValue* applyArith(JITValue* value1, JITValue* value2, bool intonly, bool floatonly, bool overflowcheck);

   JITFunction& mFunction;
};

#endif // JIT_METHOD_BUILDER_H
