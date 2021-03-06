
#include "resolver.h"

#include <stdexcept>

#include "core/streams/filereaderstream.h"
#include "core/string/string.h"
#include "core/string/stringinterface.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/file.h"

#include <libcil/common/type.h>
#include <libcil/common/types.h>

#include "mod_yas/vm/virtualclass.h"
#include "mod_yas/vm/virtualcontext.h"
#include "mod_yas/vm/virtualfield.h"

namespace ByteCode
{
   Resolver::Resolver(VirtualContext& context):
      mContext(context)
   {
   }

   // - Resolving

   VirtualClass& Resolver::resolveClass(const String& classname)
   {
      return mContext.resolveClass(classname);
   }

   VirtualFunction& Resolver::resolveFunction(const String& prototype)
   {
      std::size_t pos = prototype.lastIndexOf('(');
      pos = prototype.lastIndexOf('.', 0, pos);
      if ( pos != String::npos )
      {
         String classname = prototype.subStr(0, pos);
         String funcproto = prototype.subStr(pos + 1, prototype.length() - pos - 1);

         VirtualClass& klass = resolveClass(classname);

         pos = funcproto.indexOf('(');
         if ( pos != String::npos )
         {
            CIL::Types signature;

            String funcname = funcproto.subStr(0, pos);
            String args = funcproto.subStr(pos + 1, funcproto.length() - pos - 2);
            if ( !args.isEmpty() )
            {
               std::vector<String> arguments = StringInterface::tokenize(args, ',');
               for ( std::size_t index = 0; index < arguments.size(); ++index )
               {
                  String& arg = arguments[index];
                  signature.add(CIL::Type::fromString(arg));
               }
            }

            VirtualFunction* pfunction = klass.findExactMatch(funcname, signature);
            if ( pfunction != nullptr )
            {
               return *pfunction;
            }
            else
            {
               throw std::runtime_error("Could not find function.");
            }
         }
         else
         {
            throw std::runtime_error("Invalid prototype: no arguments");
         }
      }
      else
      {
         throw std::runtime_error("Invalid prototype: can not get class!!");
      }
   }

   VirtualField& Resolver::resolveField(const String& name)
   {
      std::size_t pos = name.lastIndexOf('.');
      if ( pos != String::npos )
      {
         String classname = name.subStr(0, pos);
         String attrname = name.subStr(pos + 1, name.length() - pos - 1);

         VirtualClass& klass = resolveClass(classname);
         VirtualClass::Fields& fields = klass.getFields();
         for ( std::size_t index = 0; index < fields.size(); ++index )
         {
            VirtualField& field = *fields[index];
            if ( field.getName() == attrname )
            {
               return field;
            }
         }

         throw std::runtime_error("Could not resolve attribute.");
      }
      else
      {
         throw std::runtime_error("Invalid attribute name.");
      }
   }

   VirtualField& Resolver::resolveStaticField(const String& name)
   {
      std::size_t pos = name.lastIndexOf('.');
      if ( pos != String::npos )
      {
         String classname = name.subStr(0, pos);
         String attrname = name.subStr(pos + 1, name.length() - pos - 1);

         VirtualClass& klass = resolveClass(classname);
         VirtualClass::Fields& fields = klass.getStaticFields();
         for ( std::size_t index = 0; index < fields.size(); ++index )
         {
            VirtualField& field = *fields[index];
            if ( field.getName() == attrname )
            {
               return field;
            }
         }

         throw std::runtime_error("Could not resolve attribute.");
      }
      else
      {
         throw std::runtime_error("Invalid attribute name.");
      }
   }

} // namespace CIL
