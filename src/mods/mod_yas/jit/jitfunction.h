
#ifndef JIT_FUNCTION_H
#define JIT_FUNCTION_H

class JITContext;
class JITSignature;

class JITFunction
{
public:
   static JITFunction* create(JITContext& context, const JITSignature& signature);

private:
   JITFunction(JITContext& context);

   void setSignature(const JITSignature& signature);

   JITContext&    mContext;
   JITSignature*  mpSignature;
};

#endif // JIT_FUNCTION_H
