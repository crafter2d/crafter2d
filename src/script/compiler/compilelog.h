
#ifndef COMPILE_LOG_H_
#define COMPILE_LOG_H_

#include <string>
#include <vector>

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

private:
   StringList mLog;
   int        mErrorCount;
};

#endif // COMPILE_LOG_H_
