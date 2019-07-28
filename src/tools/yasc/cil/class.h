
#ifndef CIL_CLASS_H
#define CIL_CLASS_H

#include <vector>

#include "core/string/string.h"

#include "../common/modifiers.h"
#include "../common/types.h"

namespace CIL
{
   class Field;
   class Function;

   class Class
   {
   public:
      typedef std::vector<Field*> Fields;
      typedef std::vector<Function*> Functions;
      typedef std::vector<String> Interfaces;

      Class();

    // get/set
      const String& getName() const;
      void          setName(const String& name);

      const String& getBaseName() const;
      void          setBaseName(const String& name);

      const Functions& getFunctions() const;
            Functions& getFunctions();

      const Fields& getFields() const;
            Fields& getFields();

      const Fields& getStatics() const;
            Fields& getStatics();

      const Interfaces& getInterfaces() const;
      Modifiers& getModifiers();

    // operations
      void add(Function* pfunction);
      void addField(Field* pfield);
      void addStatic(Field* pfield);
      void addInterface(const String& name);

   private:

    // data
      Functions   mFunctions;
      String      mName;
      String      mBaseName;
      Interfaces  mInterfaces;
      Modifiers   mModifiers;
      Fields      mFields;
      Fields      mStatics;
   };
}

#endif // CIL_CLASS_H
