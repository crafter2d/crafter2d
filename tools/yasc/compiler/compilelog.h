
#ifndef COMPILE_LOG_H_
#define COMPILE_LOG_H_

#include "core/string/stringlist.h"

class AntlrTextPosition;
class String;

class CompileLog
{
public:
   CompileLog();

 // query
   const StringList& getLog() const;
   bool hasErrors() const;

 // logging
   void warning(int errnr, const String& classname, const String& msg, const AntlrTextPosition& pos);
   void error(int errnr, const String& classname, const String& msg, const AntlrTextPosition& pos);

 // operations
   void clear();

private:
   StringList mLog;
   int        mErrorCount;
};

#endif // COMPILE_LOG_H_
