
#include "functionregistration.h"

#include "core/defines.h"

#include "classregistration.h"

// static 
FunctionRegistration* FunctionRegistration::create(const String& prototype, yas::CallbackFunctor* pcallback)
{
   return new FunctionRegistration(prototype, pcallback);
}

// - Constructors

FunctionRegistration::FunctionRegistration(const String& prototype, yas::CallbackFunctor* pcallback) :
   mpClass(NULL),
   mIndex(-1),
   mPrototype(prototype),
   mpCallback(pcallback)
{
}

FunctionRegistration::FunctionRegistration(const FunctionRegistration& that):
   mpClass(that.mpClass),
   mIndex(that.mIndex),
   mPrototype(that.mPrototype),
   mpCallback(that.mpCallback)
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

yas::CallbackFunctor& FunctionRegistration::getCallback() const
{
   return *mpCallback;
}

// - Maintenance

FunctionRegistration* FunctionRegistration::clone() const
{
   return new FunctionRegistration(*this);
}
