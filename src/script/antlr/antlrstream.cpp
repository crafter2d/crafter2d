
#include "antlrstream.h"

#include "output/asLexer.h"

// static
AntlrStream* AntlrStream::fromFile(const std::string& filename)
{
   //QByteArray bytes = filename.toUtf8();

   pANTLR3_UINT8 name  = (pANTLR3_UINT8)filename.c_str(); //bytes.data();
   pANTLR3_INPUT_STREAM input = antlr3FileStreamNew(name, ANTLR3_ENC_UTF8);
   if ( input == NULL )
   {
      return NULL;
   }

   return new AntlrStream(input);
}

// static
AntlrStream* AntlrStream::fromString(const std::string& code)
{
   //QByteArray bytes = code.toUtf8();

   pANTLR3_UINT8        name  = (pANTLR3_UINT8)"expression";
   pANTLR3_UINT8	      data  = (pANTLR3_UINT8)code.c_str(); // bytes.data();
   pANTLR3_INPUT_STREAM input = antlr3StringStreamNew(data, ANTLR3_ENC_UTF8, code.length(), name);
   if ( input == NULL )
   {
      return NULL;
   }

   return new AntlrStream(input);
}

AntlrStream::AntlrStream(ANTLR3_INPUT_STREAM_struct* pstream):
   mpStream(pstream)
{
}

AntlrStream::~AntlrStream()
{
   mpStream->close(mpStream);
}

// - Get/set

ANTLR3_INPUT_STREAM_struct* AntlrStream::getStream() const
{
   return mpStream;
}
