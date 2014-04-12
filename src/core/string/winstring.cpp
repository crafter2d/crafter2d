
#include "winstring.h"

#include <string>
#include <iconv.h>

uint32_t conv_utf8_to_utf16(wchar_t* pdest, uint32_t size, const char* psrc, uint32_t length)
{
   if ( length == 0 )
   {
      pdest[0] = 0;
      return 0;
   }

   iconv_t cd;
   cd = iconv_open("WCHAR_T", "UTF-8");
   if ( cd == (iconv_t)-1 )
   {
      pdest[0] = 0;
      return 0;
   }

   uint32_t inleft = length;
   uint32_t outleft = size * sizeof(wchar_t);
   const char* pinput = psrc;
   char* poutput = (char*) pdest;
   size_t converted = iconv (cd, (const char **) &pinput, &inleft, (char**) &poutput, &outleft);
   if ( converted == (size_t) -1 )
   {
      if ( errno == EINVAL )
      {
          // invalid sequence, we ignore the rest
      }
      else
      {

      }
   }

   iconv(cd, NULL, NULL, &poutput, &outleft);
   iconv_close(cd);

   *((wchar_t *) poutput) = L'\0';

   return (wchar_t *) poutput - pdest;
}

char* conv_utf16_to_utf8(const wchar_t* psrc, uint32_t length)
{
   iconv_t cd;
   cd = iconv_open("UTF-8", "WCHAR_T");
   if ( cd == (iconv_t)-1 )
   {
      return NULL;
   }

   uint32_t totalsize = length * sizeof(wchar_t) + 1;
   uint32_t inleft = totalsize;
   uint32_t outleft = inleft;

   char* presult = (char*) malloc(totalsize);

   const wchar_t* pinput = psrc;
   char* poutput = presult;

   do
   {
      errno = 0;

      size_t converted = iconv (cd, (const char **) &pinput, &inleft, (char**) &poutput, &outleft);
      if ( converted == (size_t)-1 )
      {
         if ( errno == E2BIG )
         {
            // need to resize the buffer
            char* ptemp = (char*) realloc(presult, totalsize + inleft);
            if ( ptemp == NULL )
            {
               free(presult);
               return NULL;
            }
            
            presult = ptemp;
            outleft += inleft;
            totalsize += inleft;
         }
         else
         {
            break;
         }
      }
   }
   while ( inleft > 0 );

   iconv(cd, NULL, NULL, &poutput, &outleft);
   iconv_close(cd);

   *poutput = '\0';

   return presult;
}
