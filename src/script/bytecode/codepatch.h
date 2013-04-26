
#ifndef CODE_PATCH_H
#define CODE_PATCH_H

namespace ByteCode
{
   class Patch
   {
   public:
      
      Patch();

    // get/set
      int  getOffset() const;
      void setOffset(int offset);

      virtual void apply(char* pcode) = 0;

   private:

      int               mOffset;
   };
}

#endif // CODE_PATCH_H
