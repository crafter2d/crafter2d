
#ifndef ANTLR_STREAM_H_
#define ANTLR_STREAM_H_

#include <string>

class String;

struct ANTLR3_INPUT_STREAM_struct;

class AntlrStream
{
public:
   static AntlrStream* fromFile(const String& filename);
   static AntlrStream* fromString(const String& code);

   ~AntlrStream();

   ANTLR3_INPUT_STREAM_struct* getStream() const;

private:
   AntlrStream(const std::string& data);
      
   mutable ANTLR3_INPUT_STREAM_struct* mpStream;
   std::string mData;
};

#endif // ANTLR_STREAM_H_
