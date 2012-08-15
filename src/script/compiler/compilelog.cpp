
#include "compilelog.h"

#include "core/string/string.h"

CompileLog::CompileLog():
   mLog(),
   mErrorCount(0)
{
}

// - Query
   
const CompileLog::StringList& CompileLog::getLog() const
{
   return mLog;
}

bool CompileLog::hasErrors() const
{
   return mErrorCount > 0;
}

// - Logging

void CompileLog::info(const std::string& info)
{
   mLog.push_back(info);
}

void CompileLog::warning(const std::string& warning)
{
   mLog.push_back("Warning: " + warning);
}

void CompileLog::error(const std::string& error)
{
   mLog.push_back("Error: " + error);

   mErrorCount++;
}

void CompileLog::warning(const String& warning)
{
   mLog.push_back("Warning: " + warning.toStdString());
}

void CompileLog::error(const String& error)
{
   mLog.push_back("Error: " + error.toStdString());

   mErrorCount++;
}

// operations

void CompileLog::clear()
{
   mLog.clear();
   mErrorCount = 0;
}