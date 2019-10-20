
#include "modifiers.h"

#include "core/defines.h"

Modifiers::Modifiers(int flags):
   mFlags(flags)
{
}

Modifiers::Modifiers(const Modifiers& that):
   mFlags(that.mFlags)
{
}
   
Modifiers& Modifiers::operator=(const Modifiers& that)
{
   mFlags = that.mFlags;
   return *this;
}

// - Get/set

bool Modifiers::isAbstract() const
{
   return IS_SET(mFlags, eAbstract);
}
      
void Modifiers::setAbstract(bool isabstract)
{
   if ( isabstract )
      SET_FLAG(mFlags, eAbstract);
   else
      CLEAR_FLAG(mFlags, eAbstract);
}

bool Modifiers::isPureNative() const
{
   return IS_SET(mFlags, ePureNative);
}

void Modifiers::setPureNative(bool ispurenative)
{
   if ( ispurenative )
      SET_FLAG(mFlags, ePureNative);
   else
      CLEAR_FLAG(mFlags, ePureNative);
}

bool Modifiers::isStatic() const
{
   return IS_SET(mFlags, eStatic);
}

void Modifiers::setStatic(bool isstatic)
{
   if ( isstatic )
      SET_FLAG(mFlags, eStatic);
   else
      CLEAR_FLAG(mFlags, eStatic);
}

// - Conversion
      
int Modifiers::toInt() const
{
   return mFlags;
}
