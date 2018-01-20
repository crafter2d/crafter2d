
#include "functionregistration.h"

#include "core/defines.h"

#include "mod_yas/common/callbackfunctor.h"

#include "classregistration.h"

// - Constructors

FunctionRegistration::FunctionRegistration():
   mPrototype(),
   mpClass(nullptr),
   mpCallback(nullptr),
   mIndex(-1)
{
}

FunctionRegistration::FunctionRegistration(const String& prototype, yas::CallbackFunctor* pcallback) :
   mPrototype(prototype),
   mpClass(nullptr),
   mpCallback(pcallback),
   mIndex(-1)
{
}

FunctionRegistration::FunctionRegistration(const FunctionRegistration& that):
   mPrototype(that.mPrototype),
   mpClass(that.mpClass),
   mpCallback(that.mpCallback),
   mIndex(that.mIndex)
{
}

FunctionRegistration::FunctionRegistration(FunctionRegistration&& source):
   mPrototype(std::move(source.mPrototype)),
   mpClass(source.mpClass),
   mpCallback(source.mpCallback),
   mIndex(source.mIndex)
{
}

FunctionRegistration& FunctionRegistration::operator=(FunctionRegistration&& source)
{
   mPrototype = std::move(source.mPrototype);
   mpClass = source.mpClass;
   mpCallback = source.mpCallback;
   mIndex = source.mIndex;
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

// - Execution

void FunctionRegistration::execute(VirtualCall& call)
{
   mpCallback->exec(call);
}

// - Maintenance

FunctionRegistration* FunctionRegistration::clone() const
{
   return new FunctionRegistration(*this);
}
