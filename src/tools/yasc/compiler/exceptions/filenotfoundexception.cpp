
#include "filenotfoundexception.h"

FileNotFoundException::FileNotFoundException(const String& filename):
   CompileException(),
   mFilename(filename)
{
}

// - Overrides

String FileNotFoundException::asString() const
{
   return UTEXT("File not found: ") + mFilename;
}
