
#include "program.h"

namespace ByteCode
{
   Program::Program():
      mSymbolTable(),
      mpCode((char*)malloc(32)),
      mCodeLength(0),
      mCodeSize(32)
   {
   }

   // - Get/set

   const SymbolTable& Program::getSymbolTable() const
   {
      return mSymbolTable;
   }

   SymbolTable& Program::getSymbolTable()
   {
      return mSymbolTable;
   }
   
   int Program::getSize() const
   {
      return mCodeSize;
   }

   const char* Program::getCode() const
   {
      return mpCode;
   }

   // - Operations

   int Program::linkCode(const char* pcode, unsigned int size)
   {
      int newsize = mCodeLength + size;
      if ( newsize > mCodeSize )
      {
         do
         {
            mCodeSize *= 2;
         }
         while ( mCodeSize < newsize );

         mpCode = (char*)realloc(mpCode, mCodeSize);
      }

      int index = mCodeLength;
      memcpy(&mpCode[index], pcode, size);
      mCodeLength += size;

      return index;
   }
}