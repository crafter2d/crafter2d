
#ifndef COMPILE_LOG_H_
#define COMPILE_LOG_H_

#include <string>
#include <vector>

class String;

class CompileLog
{
public:
   typedef std::vector<std::string> StringList;

   CompileLog();

 // query
   const StringList& getLog() const;
   bool hasErrors() const;

 // logging
   void info(const std::string& info);
   void warning(const std::string& warning);
   void error(const std::string& error);

   void warning(const String& warning);
   void error(const String& error);

 // operations
   void clear();

private:
   StringList mLog;
   int        mErrorCount;
};

#endif // COMPILE_LOG_H_
