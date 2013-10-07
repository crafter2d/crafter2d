
#include "winstring.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

uint32_t conv_utf8_to_utf16(wchar_t* pdest, uint32_t size, const char* psrc, uint32_t length)
{
   if ( length == 0 )
   {
      pdest[0] = 0;
      return 0;
   }

   int len = MultiByteToWideChar(CP_ACP, 0, psrc, length, pdest, size);

   if ( len <= 0 )
   {
      DWORD err = ::GetLastError();
      switch ( err )
      {
      case ERROR_INSUFFICIENT_BUFFER:
         return -1;
      case ERROR_INVALID_FLAGS:
         return -2;
      case ERROR_INVALID_PARAMETER:
         return -3;
      case ERROR_NO_UNICODE_TRANSLATION:
         return -4;
      default:
         return -5;
      }
      len = 0;
   }

   if ( len <= length )
   {
      pdest[len] = 0;
   }
   else
   {
      pdest[length] = 0;
   }

   return len;
}

char* conv_utf16_to_utf8(const wchar_t* psrc, uint32_t length)
{
   int utf8len = WideCharToMultiByte(CP_UTF8, 0, psrc, length, NULL, 0, NULL, NULL);

   if ( utf8len <= 0 )
   {
      return NULL;
   }

   char* presult = new char[utf8len + 1]; // include eos

   WideCharToMultiByte(CP_UTF8, 0, psrc, length, presult, utf8len, NULL, NULL);

   presult[utf8len] = 0;

   return presult;
}
