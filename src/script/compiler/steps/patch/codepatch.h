
#ifndef CODE_PATCH_H
#define CODE_PATCH_H

class VirtualFunction;

class CodePatch
{
public:
   enum Kind
   {
      eCallInterface
   };

   CodePatch(Kind kind);

 // get/set
   Kind getKind() const;

   int  getOffset() const;
   void setOffset(int offset);

   VirtualFunction& getVirtualFunction();
   void             setVirtualFunction(VirtualFunction& function);

private:

   Kind              mKind;
   int               mOffset;
   VirtualFunction*  mpVirFunction;
};

#endif // CODE_PATCH_H
