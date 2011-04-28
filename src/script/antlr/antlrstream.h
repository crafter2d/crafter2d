
#ifndef ANTLR_STREAM_H_
#define ANTLR_STREAM_H_

#include <string>

struct ANTLR3_INPUT_STREAM_struct;

class AntlrStream
{
public:
   static AntlrStream* fromFile(const std::string& filename);
   static AntlrStream* fromString(const std::string& code);

   ~AntlrStream();

   ANTLR3_INPUT_STREAM_struct* getStream() const;

private:
   AntlrStream(ANTLR3_INPUT_STREAM_struct* pstream);

   ANTLR3_INPUT_STREAM_struct* mpStream;
};

#endif // ANTLR_STREAM_H_
