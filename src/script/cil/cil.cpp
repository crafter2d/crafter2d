
#include "cil.h"

#include "core/string/string.h"

static String sBool("bool");
static String sInt("int");
static String sReal("real");
static String sChar("char");
static String sString("string");
static String sVoid("void");

namespace CIL
{
   // static 
   String Type::toString(const Type& type)
   {
      switch ( type.type )
      {
         case eBool:
            return sBool;
         case eInt:
            return sInt;
         case eReal:
            return sReal;
         case eChar:
            return sChar;
         case eString:
            return sString;
         case eObject:
            return *type.name;
         case eArray:
            return toString(*type.elem_type) + "[]";
         case eVoid:
            return sVoid;
      }
   }
   
   // static 
   Type* Type::fromString(const String& type)
   {
      Type* presult = new Type();

      int index = type.lastIndexOf('[');
      if ( index > 0 )
      {
         // array type
         String elemtype = type.subStr(0, index);
         presult->elem_type = fromString(elemtype);
         presult->type = eArray;
      }
      else if ( type == sBool )
      {
         presult->type == eBool;
      }
      else if ( type == sInt )
      {
         presult->type = eInt;
      }
      else if ( type == sReal )
      {
         presult->type = eReal;
      }
      else if ( type == sChar )
      {
         presult->type = eChar;
      }
      else if ( type == sString )
      {
         presult->type = eString;
      }
      else if ( type == sVoid )
      {
         presult->type = eVoid;
      }
      else
      {
         presult->name = new String(type);
         presult->type = eObject;
      }

      return presult;
   }

   bool Type::match(const Type& that) const
   {
      if ( type == that.type )
      {
         switch ( type )
         {
            case eObject:
               return *name == *that.name;
            case eArray:
               return elem_type->match(*that.elem_type);
            default:
               return true;
         }
      }

      return false;
   }
}
