
#include "compilelog.h"

#include "core/string/string.h"

#include "antlr/antlrtextposition.h"

CompileLog::CompileLog():
   mLog(),
   mErrorCount(0)
{
}

// - Query
   
const StringList& CompileLog::getLog() const
{
   return mLog;
}

bool CompileLog::hasErrors() const
{
   return mErrorCount > 0;
}

// - Logging

void CompileLog::warning(int errnr, const String& classname, const String& msg, const AntlrTextPosition& pos)
{
   String warn = UTEXT("{0}({1}): warning {2}: {3}").arg(0, classname).arg(1, pos.getLine()).arg(2, errnr).arg(3, msg);
   mLog.add(warn);
}

void CompileLog::error(int errnr, const String& classname, const String& msg, const AntlrTextPosition& pos)
{
   String err = UTEXT("{0}({1}): error {2}: {3}").arg(0, classname).arg(1, pos.getLine()).arg(2, errnr).arg(3, msg);
   mLog.add(err);
   mErrorCount++;
}

// operations

void CompileLog::clear()
{
   mLog.clear();
   mErrorCount = 0;
}