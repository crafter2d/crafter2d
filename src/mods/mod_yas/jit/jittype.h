
#ifndef JIT_TYPE_H
#define JIT_TYPE_H

#include <vector>
#include "jit-common.h"

class JITType
{
public:
   enum Kind {
      eInvalid,
      eVoid,
      eByte,
      eUByte,
      eShort,
      eUShort,
      eInt,
      eUInt,
      eLong,
      eULong,
      eFloat32,
      eFloat64,
      ePointer,
      eSignature
   };

   enum Layout {
      eLayoutNeeded = 1,
      eLayoutExplicitSize = 2,
      eLayoutExplicitAlign = 4
   };

   struct JITComponent
   {
      JITType* ptype;
      unsigned int offset;
      char* pname;
   };

   JITType();

   JITType& operator=(const JITType& that);

 // creation
   static JITType* createComplex(JITType::Kind kind, std::vector<JITType*>& types);
   static JITType* createSignature(JIT::Abi abi, JITType* preturntype, std::vector<JITType*>& params);

 // operations
   JITType* clone() const;

private:
   typedef std::vector<JITComponent> Components;

   JITType(const JITType& that);

 // operations
   void assignComponents(const std::vector<JITType::JITComponent>& components);

   Kind         mKind;
   int          mLayoutFlags;
   JIT::Abi     mAbi;
   unsigned int mSize;
   JITType*     mpSubType;
   Components   mComponents;
};

#endif // JIT_TYPE_H
