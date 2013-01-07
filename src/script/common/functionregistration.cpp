
#include "functionregistration.h"

// static 
FunctionRegistration FunctionRegistration::Constructor(const String& prototype, VMInterface::CallbackFnc callback)
{
   return FunctionRegistration(eConstructor, prototype, callback);
}

// static 
FunctionRegistration FunctionRegistration::Destructor(VMInterface::CallbackFnc callback)
{
   return FunctionRegistration(eDestructor, callback);
}

// static 
FunctionRegistration FunctionRegistration::Function(const String& prototype, VMInterface::CallbackFnc callback)
{
   return FunctionRegistration(eFunction, prototype, callback);
}

// - Constructors

FunctionRegistration::FunctionRegistration(Type type, VMInterface::CallbackFnc callback):
   mIndex(-1),
   mType(type),
   mPrototype(),
   mCallback(callback)
{
}

FunctionRegistration::FunctionRegistration(Type type, const String& prototype, VMInterface::CallbackFnc callback):
   mIndex(-1),
   mType(type),
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

const FunctionRegistration& FunctionRegistration::operator=(const FunctionRegistration& that)
{
   mIndex = that.mIndex;
   mType = that.mType;
   mPrototype = that.mPrototype;
   mCallback = that.mCallback;
   return *this;
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

FunctionRegistration::Type FunctionRegistration::getType() const
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
