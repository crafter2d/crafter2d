
#include "functionregistration.h"

// static 
FunctionRegistration* FunctionRegistration::create(const String& prototype, VMInterface::CallbackFnc callback)
{
   return new FunctionRegistration(prototype, callback);
}

// - Constructors

FunctionRegistration::FunctionRegistration(const String& prototype, VMInterface::CallbackFnc callback):
   mIndex(-1),
   mType(eInvalidType),
   mPrototype(prototype),
   mCallback(callback)
{
}

FunctionRegistration::FunctionRegistration(const FunctionRegistration& that):
   mIndex(that.mIndex),
   mType(that.mType),
   mPrototype(that.mPrototype),
   mCallback(that.mCallback)
{
}

// - Get/set

int FunctionRegistration::getIndex() const
{
   return mIndex;
}

void FunctionRegistration::setIndex(int index)
{
   mIndex = index;
}

FunctionRegistration::Type FunctionRegistration::getReturnType() const
{
   return mType;
}

const String& FunctionRegistration::getPrototype() const
{
   return mPrototype;
}

VMInterface::CallbackFnc FunctionRegistration::getCallback() const
{
   return mCallback;
}

// - Maintenance

FunctionRegistration* FunctionRegistration::clone() const
{
   return new FunctionRegistration(*this);
}
