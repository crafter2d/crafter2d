
#ifndef CLASS_NOT_FOUND_EXCEPTION_H_
#define CLASS_NOT_FOUND_EXCEPTION_H_

#include <string>

class ClassNotFoundException
{
public:
   ClassNotFoundException(const std::string& classname): mClass(classname) {}

   std::string mClass;
};

#endif // CLASS_NOT_FOUND_EXCEPTION_H_
