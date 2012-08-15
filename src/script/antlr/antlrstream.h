
#ifndef ANTLR_STREAM_H_
#define ANTLR_STREAM_H_

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
   AntlrStream(ANTLR3_INPUT_STREAM_struct* pstream, char* pdata);
      
   ANTLR3_INPUT_STREAM_struct* mpStream;
   char* mpData;
};

#endif // ANTLR_STREAM_H_
