
#ifndef CLASS_REGISTRATION_H
#define CLASS_REGISTRATION_H

#include <vector>

#include "core/string/string.h"

class FunctionRegistration;

class ClassRegistration
{
public:
   typedef std::vector<FunctionRegistration*> Functions;

   ClassRegistration(const String& name, int start, int end) :
      mName(name),
      mFunctions(),
      mStart(start),
      mEnd(end)
   {
   }

   ClassRegistration(const ClassRegistration& that, size_t offset = 0) :
      mName(that.mName),
      mFunctions(that.mFunctions),
      mStart(that.mStart + offset),
      mEnd(that.mEnd + offset)
   {
   }

   ClassRegistration(ClassRegistration&& that):
      mName(std::move(that.mName)),
      mFunctions(std::move(that.mFunctions)),
      mStart(that.mStart),
      mEnd(that.mEnd)
   {
   }

   ~ClassRegistration()
   {
   }

   ClassRegistration& operator=(ClassRegistration&& source)
   {
      mName = std::move(source.mName);
      mFunctions = std::move(source.mFunctions);
      mStart = source.mStart;
      mEnd = source.mEnd;
      return *this;
   }

   void addFunction(FunctionRegistration& registration)
   {
      mFunctions.push_back(&registration);
      mEnd++;
   }

   String mName;
   Functions mFunctions; // not owning
   int mStart;
   int mEnd;
};

#endif // CLASS_REGISTRATION_H
