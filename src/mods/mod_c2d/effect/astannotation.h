
#ifndef AST_ANNOTATION_H
#define AST_ANNOTATION_H

#include "core/string/string.h"

class ASTAnnotation
{
public:
   enum Type { eString, eNumber, eNoValue };

   ASTAnnotation();

 // get/set
   void setValue(const String& value);
   void setValue(int value);

 // data
   String name;
   Type type;
   int intvalue;
   String strvalue;
};

#endif // AST_ANNOTATION_H
