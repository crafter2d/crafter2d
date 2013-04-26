
#include "functionregistration.h"

#include "core/defines.h"

#include "classregistration.h"

// static 
FunctionRegistration* FunctionRegistration::create(const String& prototype, VMInterface::CallbackFnc callback)
{
   return new FunctionRegistration(prototype, callback);
}

// - Constructors

FunctionRegistration::FunctionRegistration(const String& prototype, VMInterface::CallbackFnc callback):
   mpClass(NULL),
   mIndex(-1),
   mPrototype(prototype),
   mCallback(callback)
{
}

FunctionRegistration::FunctionRegistration(const FunctionRegistration& that):
   mpClass(that.mpClass),
   mIndex(that.mIndex),
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

const ClassRegistration& FunctionRegistration::getClassRegistration() const
{
   ASSERT_PTR(mpClass);
   return *mpClass;
}

void FunctionRegistration::setClassRegistration(const ClassRegistration& klass)
{
   mpClass = &klass;
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
