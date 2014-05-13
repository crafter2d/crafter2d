
#include "astannotation.h"

ASTAnnotation::ASTAnnotation():
   name(),
   type(eNoValue),
   intvalue(),
   strvalue()
{
}

// - Get/set

void ASTAnnotation::setValue(const String& value)
{
   strvalue = value;
   type = eString;
}

void ASTAnnotation::setValue(int value)
{
   intvalue = value;
   type = eNumber;
}
