
#ifndef AST_STREAM_H_
#define AST_STREAM_H_

#include <sstream>

class ASTClass;
class ASTField;
class ASTFunction;
class ASTModifiers;
class ASTType;
class ASTTypeList;
class ASTTypeVariables;
class ASTTypeVariable;
class ASTVariable;

class ASTStream
{
public:
   explicit ASTStream(std::stringstream& stream);

 // streaming
   ASTStream& operator<<(const ASTClass& ast);
   ASTStream& operator<<(const ASTField& ast);
   ASTStream& operator<<(const ASTFunction& ast);

   ASTStream& operator>>(ASTClass& ast);
   ASTStream& operator>>(ASTField& ast);
   ASTStream& operator>>(ASTFunction& ast);

private:
 // streamer
   void writeModifiers(const ASTModifiers& modifiers);
   void writeTypeVariables(const ASTTypeVariables& variables);
   void writeTypeVariable(const ASTTypeVariable& variable);
   void writeType(const ASTType& type);
   void writeTypeList(const ASTTypeList& types);
   void writeVariable(const ASTVariable& variable);

   void readModifiers(ASTModifiers& modifiers);
   void readTypeVariables(ASTTypeVariables& variables);
   void readTypeVariable(ASTTypeVariable& variable);
   void readTypeList(ASTTypeList& types);
   ASTType* readType();
   ASTVariable* readVariable();

   std::stringstream& mStream;
};

#endif // AST_STREAM_H_
