
#include "jittype.h"


#define	JIT_OFFSET_IS_INTERNAL	(((unsigned int)1) << (sizeof(unsigned int) * 8 - 1))
#define	JIT_OFFSET_NOT_SET		(~((unsigned int)0))

JITType::JITType():
   mKind(eInvalid),
   mLayoutFlags(0),
   mAbi(),
   mSize(0),
   mpSubType(nullptr),
   mComponents()
{
}

JITType::JITType(const JITType& that):
   mKind(that.mKind),
   mLayoutFlags(that.mLayoutFlags),
   mAbi(that.mAbi),
   mSize(that.mSize),
   mpSubType(nullptr),
   mComponents()
{
   assignComponents(that.mComponents);
}

JITType& JITType::operator=(const JITType& that)
{
   mKind = that.mKind;
   mLayoutFlags = that.mLayoutFlags;
   mAbi = that.mAbi;
   mSize = that.mSize;
   mpSubType = that.mpSubType;
   assignComponents(that.mComponents);
   return *this;
}

// - Static creation functions

JITType* JITType::createComplex(JITType::Kind kind, std::vector<JITType*>& types)
{
   JITType* ptype = new JITType();
   if ( ptype != nullptr )
   {
      ptype->mKind = kind;
      ptype->mLayoutFlags = eLayoutNeeded;
      ptype->mComponents.reserve(types.size());

      for ( unsigned int index = 0; index < types.size(); ++index )
      {
         JITType::JITComponent& component = ptype->mComponents[index];
         component.ptype = types[index];
         component.offset = JIT_OFFSET_NOT_SET;
         component.pname = nullptr;
      }
   }

   return ptype;
}

JITType* JITType::createSignature(JIT::Abi abi, JITType* preturntype, std::vector<JITType*>& params)
{
   JITType* ptype = createComplex(eSignature, params);
   if ( ptype != nullptr )
   {
      ptype->mAbi = abi;
      ptype->mLayoutFlags = 0;
      ptype->mSize = 0;
      //ptype->mAlignment = 
      ptype->mpSubType = preturntype;
   }

   return ptype;
}

// - Operations

JITType* JITType::clone() const
{
   return new JITType(*this);
}

void JITType::assignComponents(const std::vector<JITType::JITComponent>& components)
{
   for ( unsigned int index = 0; index < components.size(); ++index )
   {
      const JITType::JITComponent& component = components[index];
      mComponents.push_back(component);
   }
}
