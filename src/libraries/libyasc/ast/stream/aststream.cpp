
#include "aststream.h"

#include "libyasc/ast/ast.h"

ASTStream::ASTStream(String& stream):
   mStream(stream)
{
}

// - Streaming

/*
ASTStream& ASTStream::operator<<(const ASTClass& ast)
{
   mStream << ast.getName();
   mStream << ast.getKind();
   mStream << ast.hasBaseType();

   if ( ast.hasBaseType() )
   {
      writeType(ast.getBaseType());
   }

   writeModifiers(ast.getModifiers());
   writeTypeVariables(ast.getTypeVariables());
   writeTypeList(ast.getInterfaces());

   return *this;
}

ASTStream& ASTStream::operator<<(const ASTField& ast)
{
   writeVariable(ast.getVariable());

   return *this;
}

ASTStream& ASTStream::operator<<(const ASTFunction& ast)
{
   mStream << ast.getName();
   mStream << ast.getResourceIndex();

   writeModifiers(ast.getModifiers());
   writeType(ast.getType());

   const ASTNodes& arguments = ast.getArguments();
   mStream << arguments.size();
   for ( int index = 0; index < arguments.size(); index++ )
   {
      const ASTFunctionArgument& argument = dynamic_cast<const ASTFunctionArgument&>(arguments[index]);
      writeVariable(argument.getVariable());
   }

   return *this;
}

ASTStream& ASTStream::operator>>(ASTClass& ast)
{
   std::string name;
   int kind;
   bool hasbase;

   mStream >> name >> kind >> hasbase;
   if ( hasbase )
   {
      ast.setBaseType(readType());
   }

   ast.setName(name);
   ast.setKind((ASTClass::Kind)kind);

   readModifiers(ast.getModifiers());
   readTypeVariables(ast.getTypeVariables());
   readTypeList(ast.getInterfaces());

   return *this;
}

ASTStream& ASTStream::operator>>(ASTField& ast)
{
   ast.setVariable(readVariable());

   return *this;
}

ASTStream& ASTStream::operator>>(ASTFunction& ast)
{
   std::string name;
   int resourceindex;

   mStream >> name >> resourceindex;
   readModifiers(ast.getModifiers());

   ast.setName(name);
   ast.setResourceIndex(resourceindex);
   ast.setType(readType());
   
   int size;
   mStream >> size;
   for ( int index = 0; index < size; index++ )
   {
      ASTFunctionArgument* pargument = new ASTFunctionArgument(readVariable());
   }

   return *this;
}

// - Helpers

void ASTStream::writeModifiers(const ASTModifiers& modifiers)
{
   mStream << modifiers.getVisibility();
   mStream << modifiers.getFlags();
}

void ASTStream::writeTypeVariables(const ASTTypeVariables& variables)
{
   mStream << variables.size();
   
   ASTTypeVariables::TypeVariableMap::const_iterator it = variables.mTypeVariables.begin();
   for ( ; it != variables.mTypeVariables.end(); it++ )
   {
      writeTypeVariable(*it->second);
   }
}

void ASTStream::writeTypeVariable(const ASTTypeVariable& variable)
{
   mStream << variable.getIdentifier();
   mStream << variable.getMode();

   writeType(variable.getOther());
}

void ASTStream::writeType(const ASTType& type)
{
   mStream << type.getKind();
   if ( type.isObject() )
   {
      mStream << type.getObjectName();
   }
   else if ( type.isArray() )
   {
      mStream << type.getArrayDimension();
      
      writeType(type.getArrayType());
   }
}

void ASTStream::writeTypeList(const ASTTypeList& types)
{
   mStream << types.size();
   for ( int index = 0; index < types.size(); index++ )
   {
      writeType(types[index]);
   }
}

void ASTStream::writeVariable(const ASTVariable& variable)
{
   mStream << variable.getName();
   mStream << variable.getResourceIndex();

   writeModifiers(variable.getModifiers());
   writeType(variable.getType());
}

void ASTStream::readModifiers(ASTModifiers& modifiers)
{
   int vis, flags;
   mStream >> vis >> flags;

   modifiers.setVisibility((ASTModifiers::Access)vis);
   modifiers.setFlags(flags);
}

void ASTStream::readTypeVariables(ASTTypeVariables& variables)
{
   int size = 0;
   mStream >> size;

   for ( int index = 0; index < size; index++ )
   {
      ASTTypeVariable* pvariable = new ASTTypeVariable();
      readTypeVariable(*pvariable);

      variables.append(pvariable);
   }
}

void ASTStream::readTypeVariable(ASTTypeVariable& variable)
{
   std::string id;
   int mode;

   mStream >> id >> mode;

   variable.setIdentifier(id);
   variable.setMode((ASTTypeVariable::Mode)mode);

   if ( mode != ASTTypeVariable::eNormal )
   {
      variable.setOther(readType());
   }
}

void ASTStream::readTypeList(ASTTypeList& types)
{
   int size = 0;
   mStream >> size;

   for ( int index = 0; index < size; index++ )
   {
      types.append(readType());
   }
}

ASTType* ASTStream::readType()
{
   int kind;
   mStream >> kind;

   ASTType* ptype = new ASTType((ASTType::Kind)kind);

   if ( ptype->isObject() )
   {
      std::string name;
      mStream >> name;

      ptype->setObjectName(name);
   }
   else if ( ptype->isArray() )
   {
      int dimension;
      mStream >> dimension;
      
      ptype->setArrayDimension(dimension);
      ptype->setArrayType(readType());
   }
   
   return ptype;
}

ASTVariable* ASTStream::readVariable()
{
   std::string name;
   int resourceindex;

   mStream >> name >> resourceindex;

   ASTVariable* pvariable = new ASTVariable();

   readModifiers(pvariable->getModifiers());

   pvariable->setName(name);
   pvariable->setResourceIndex(resourceindex);
   pvariable->setType(readType());

   return pvariable;
}
*/