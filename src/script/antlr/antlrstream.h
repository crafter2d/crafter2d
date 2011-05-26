
#ifndef ANTLR_STREAM_H_
#define ANTLR_STREAM_H_

#include <string>

class String;

struct ANTLR3_INPUT_STREAM_struct;

class AntlrStream
{
public:
   static AntlrStream* fromFile(const std::string& filename);
   static AntlrStream* fromString(const String& code);

   ~AntlrStream();

   ANTLR3_INPUT_STREAM_struct* getStream() const;

private:
   AntlrStream(ANTLR3_INPUT_STREAM_struct* pstream, char* pdata);
      
   ANTLR3_INPUT_STREAM_struct* mpStream;
   char* mpData;
};

#endif // ANTLR_STREAM_H_
