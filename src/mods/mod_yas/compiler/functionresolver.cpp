
#include "functionresolver.h"

#include "core/string/string.h"
#include "core/string/stringinterface.h"

#include "script/ast/ast.h"

#include "compilecontext.h"

ASTFunction& FunctionResolver::resolve(CompileContext& context, const String& prototype)
{
   int pos = prototype.lastIndexOf('(');
   pos = prototype.lastIndexOf('.', 0, pos);
   if ( pos != -1 )
   {
      String classname = prototype.subStr(0, pos);
      String funcproto = prototype.subStr(pos + 1, prototype.length() - pos - 1);

      ASTClass& astclass = context.resolveClass(classname);

      pos = funcproto.indexOf('(');
      if ( pos > 0 )
      {
         String funcname = funcproto.subStr(0, pos);
         String args = funcproto.subStr(pos + 1, funcproto.length() - pos - 2);
         std::vector<String> arguments = StringInterface::tokenize(args, ',');
         ASTSignature signature;
         for ( std::size_t index = 0; index < arguments.size(); ++index )
         {
            String& arg = arguments[index];
            ASTType* ptype = ASTType::fromString(arg);
            if ( ptype->isObject() )
            {
               ptype->setObjectClass(context.resolveClass(ptype->getObjectName()));
            }
            signature.append(ptype);
         }

         ASTFunction* pfunction = astclass.findExactMatch(funcname, signature);
         if ( pfunction != NULL )
         {
            return *pfunction;
         }
         else
         {
            throw std::exception("Could not find function.");
         }
      }
      else
      {
         throw std::exception("Invalid prototype: no arguments");
      }
   }
   else
   {
      throw std::exception("Invalid prototype: can not get class!!");
   }
}
