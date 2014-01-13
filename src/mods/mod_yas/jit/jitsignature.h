
#ifndef JIT_SIGNATURE_H
#define JIT_SIGNATURE_H

#include <vector>

class JITType;

class JITSignature
{
   typedef std::vector<JITType*> Types;

public:
   JITSignature();

   void add(JITType* ptype);

   JITSignature* clone() const;

private:

   Types mTypes;
};

#endif // JIT_SIGNATURE_H
