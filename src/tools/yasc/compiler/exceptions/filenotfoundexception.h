
#ifndef FILE_NOT_FOUND_EXCEPTION_H
#define FILE_NOT_FOUND_EXCEPTION_H

#include "core/string/string.h"

#include "compileexception.h"

class FileNotFoundException : public CompileException
{
public:
   FileNotFoundException(const String& filename);

 // query
   virtual String asString() const override;

private:

 // data
   String mFilename;
};

#endif // FILE_NOT_FOUND_EXCEPTION_H
