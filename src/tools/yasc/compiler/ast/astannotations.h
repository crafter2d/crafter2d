
#ifndef AST_ANNOTATIONS_H_
#define AST_ANNOTATIONS_H_

#include <vector>

#include "core/string/string.h"

#include "astnode.h"

class ASTAnnotation;

class ASTAnnotations : public ASTNode
{
   typedef std::vector<ASTAnnotation*> Annotations;

public:
   ASTAnnotations();

   const ASTAnnotation& operator[](int index) const;

 // query
   int size() const;

   bool contains(const String& name) const;

 // operations
   void add(ASTAnnotation* pannotation);

 // visitor
   ACCEPT;

private:
   Annotations mAnnotations;
};

#endif // AST_ANNOTATIONS_H_
