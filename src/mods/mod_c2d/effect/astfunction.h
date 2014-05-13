
#ifndef AST_FUNCTION_H
#define AST_FUNCTION_H

#include "astnode.h"
#include "astannotation.h"

#include <vector>

class ASTFunctionArgument;
class ASTType;

class ASTFunction : public ASTNode
{
public:
   typedef std::vector<ASTFunctionArgument*> Arguments;
   typedef std::vector<ASTAnnotation*> Annotations;

 // operations
   void addArgument(ASTFunctionArgument* parg) { mArguments.push_back(parg); }
   void addAnnotation(ASTAnnotation* panno) { mAnnotations.push_back(panno); }

 // search
   const ASTAnnotation* findAnnotation(const String& name) const {
      for ( std::size_t index = 0; index < mAnnotations.size(); ++index ) {
         const ASTAnnotation* panno = mAnnotations[index];
         if ( panno->name == name )
            return panno;
      }
      return NULL;
   }

 // data   
   ASTType*       mpType;
   String         mName;
   String         mBody;
   String         mReturn;
   Arguments      mArguments;
   Annotations    mAnnotations;
};

#endif // AST_FUNCTION_H
