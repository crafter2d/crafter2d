

#include "astfunction.h"

#include "astclass.h"

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

void ASTFunction::doGenerateCodeBegin(FILE* out, CodePhase phase)
{
   if ( phase == eFirst )
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

      fprintf(out, "void %s(ScriptLibContext& context)\n{\n", mFunction.c_str());
      fprintf(out, "}\n");
   }
   else
   {
      switch ( mType )
      {
         case eConstructor:
            fprintf(out, "   theclass.addFunction(\"new\", %s);\n", mFunction.c_str());
            break;
         case eDestructor:
            fprintf(out, "   theclass.addFunction(\"destroy\", %s\n);\n", mFunction.c_str());
            return;
         case eMember:
            fprintf(out, "   theclass.addFunction(\"%s\", %s);\n", mName.c_str(), mFunction.c_str());
            break;
         default:
            cout << "<unknown function>";
            break;
      }
   }
}
