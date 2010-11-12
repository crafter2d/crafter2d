

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

   stream << CodeBlockedStream::endline;
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

   stream << "void " << mFunction << "(ScriptLibContext& context)" << CodeBlockedStream::endline << "{" << CodeBlockedStream::endline;
   {
      CodeBlockedStream block(stream);

      if ( mConst )
         block << "const ";

      block << classname << "* self = (" << classname << "*)context.getUserArgument(1);" << CodeBlockedStream::endline;
   
      ASTNode::Children& children = mpArguments->getChildren();
      for ( int index = 0; index < children.size(); index++ )
      {
         ASTVariable* pvariable = dynamic_cast<ASTVariable*>(children[index]);
         ASTType& type = pvariable->getType();

         switch ( type.getType() )
         {
            case ASTType::eInt:     
               block << "int " << pvariable->getName() << " = context.getIntArgument(";
               break;
            case ASTType::eFloat:
               block << "float " << pvariable->getName() << " = context.getFloatArgument(";
               break;
            case ASTType::eBool:
               block << "bool " << pvariable->getName() << " = context.getBoolArgument(";
               break;
         }

         block << index+1 << ");" << CodeBlockedStream::endline;
      }

      bool hasresult = mpType != 0 && mpType->getType() != ASTType::eVoid;
      if ( hasresult )
      {
         std::string type = mpType->asString();
         block << type << " result = ";
      }

      block << "self->" << mName << "(";

      for ( int index = 0; index < children.size(); index++ )
      {
         ASTVariable* pvariable = dynamic_cast<ASTVariable*>(children[index]);
         block << pvariable->getName();
         if ( index < children.size() - 1 )
            block << ", ";
      }

      block << ");" << CodeBlockedStream::endline;

      if ( hasresult )
      {
         switch ( mpType->getType() )
         {
            case ASTType::eInt:
            case ASTType::eFloat:
            case ASTType::eBool:
            case ASTType::eChar:
               block << "context.setResult(result);";
               break;
            case ASTType::eCustom:
               block << "context.setResult(" << (mpType->isPointer() ? "" : "&") << "result, \"" << mpType->getCustomType() << "\");";
               break;
         }

         block << "" << CodeBlockedStream::endline;
      }
   }

   stream << "}" << CodeBlockedStream::endline;
}
