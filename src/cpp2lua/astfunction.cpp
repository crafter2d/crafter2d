

#include "astfunction.h"

#include "astclass.h"

#include "codestream.h"
#include "codeblock.h"

bool ASTFunction::doValidate()
{
   bool valid = false;
   switch ( mType )
   {
      case eConstructor:
      case eDestructor:
         {
            ASTClass& parent = dynamic_cast<ASTClass&>(getParent().getParent()); // body -> class
            valid = mName == parent.getName();
         }
         break;
      case eMember:
         valid = mpType->validate();
         break;
      default:
         break;
   }

   if ( mpArguments != 0 )
      valid = mpArguments->validate() | valid;

   return valid;
}

void ASTFunction::doGenerateCodeBegin(CodeStream& stream, CodePhase phase)
{
   if ( phase == eFirstPhase )
   {
      generateImplementation(stream);
   }
   else
   {
      generateDeclaration(stream);  
   }
}

void ASTFunction::generateDeclaration(CodeStream& stream)
{
   switch ( mType )
   {
      case eConstructor:
         stream << "theclass.addFunction(\"new\", " << mFunction << ");";
         break;
      case eDestructor:
         stream << "theclass.addFunction(\"destroy\", " << mFunction << ");";
         return;
      case eMember:
         stream << "theclass.addFunction(\"" << mName << "\", " << mFunction << ");";
         break;
   }

   stream << CodeBlock::endl;
}

void ASTFunction::generateImplementation(CodeStream& stream)
{
   std::string classname = (dynamic_cast<ASTClass&>(getParent().getParent())).getName();
   std::string name;

   switch ( mType )
   {
      case eConstructor:   name = "new";     break;
      case eDestructor:    name = "destroy"; break;
      case eMember:        name = mName;     break;
   }

   mFunction = "cpplua_" + classname + "_" + name;

   stream << "void " << mFunction << "(ScriptLibContext& context)" << CodeBlock::endl << "{";
   {
      CodeBlock block;
      stream << CodeBlock::endl;

      if ( mConst )
         stream << "const ";

      stream << classname << "* self = (" << classname << "*)context.getUserArgument(1);" << CodeBlock::endl;
   
      ASTNode::Children& children = mpArguments->getChildren();
      for ( int index = 0; index < children.size(); index++ )
      {
         ASTVariable* pvariable = dynamic_cast<ASTVariable*>(children[index]);
         ASTType& type = pvariable->getType();

         switch ( type.getType() )
         {
            case ASTType::eInt:     
               stream << "int " << pvariable->getName() << " = context.getIntArgument(";
               break;
            case ASTType::eFloat:
               stream << "float " << pvariable->getName() << " = context.getFloatArgument(";
               break;
            case ASTType::eBool:
               stream << "bool " << pvariable->getName() << " = context.getBoolArgument(";
               break;
         }

         stream << index+1 << ");" << CodeBlock::endl;
      }

      bool hasresult = mpType != 0 && mpType->getType() != ASTType::eVoid;
      if ( hasresult )
      {
         std::string type = mpType->asString();
         stream << type << " result = ";
      }

      stream << "self->" << mName << "(";

      for ( int index = 0; index < children.size(); index++ )
      {
         ASTVariable* pvariable = dynamic_cast<ASTVariable*>(children[index]);
         stream << pvariable->getName();
         if ( index < children.size() - 1 )
            stream << ", ";
      }

      stream << ");" << CodeBlock::endl;

      if ( hasresult )
      {
         switch ( mpType->getType() )
         {
            case ASTType::eInt:
            case ASTType::eFloat:
            case ASTType::eBool:
            case ASTType::eChar:
               stream << "context.setResult(result);";
               break;
            case ASTType::eCustom:
               stream << "context.setResult(" << (mpType->isPointer() ? "" : "&") << "result, \"" << mpType->getCustomType() << "\");";
               break;
         }
      }
   }

   stream << CodeBlock::endl << "}" << CodeBlock::endl;
}
