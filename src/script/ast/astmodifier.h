
#ifndef AST_MODIFIERS_H_
#define AST_MODIFIERS_H_

class ASTModifiers
{
public:
   enum Access { ePublic, eProtected, ePrivate };
   enum Flags { 
       eStatic       = 1,
       eFinal        = 2,
       eAbstract     = 4,
       eNative       = 8,
       eSynchronized = 16
   };

   ASTModifiers(): mAccess(ePublic), mFlags(0) {}

   const ASTModifiers& operator=(const ASTModifiers& that)
   {
      mAccess = that.mAccess;
      mFlags = that.mFlags;
      return *this;
   }

 // get/set
   bool isStatic() const { return (mFlags & eStatic) == eStatic; }
   void setStatic() { mFlags |= eStatic; }

   bool isFinal() const { return (mFlags & eFinal) == eFinal; }
   void setFinal() { mFlags |= eFinal; }

   bool isAbstract() const { return (mFlags & eAbstract) == eAbstract; }
   void setAbstract() { mFlags |= eAbstract; }

   bool isNative() const { return (mFlags & eNative) == eNative; }
   void setNative() { mFlags |= eNative; }

   bool isSynchronized() const { return (mFlags & eSynchronized) == eSynchronized; }
   void setSynchronized() { mFlags |= eSynchronized; }

   bool isPublic() const { return mAccess == ePublic; }
   bool isProtected() const { return mAccess == eProtected; }
   bool isPrivate() const { return mAccess == ePrivate; }

   int getFlags() const { return mFlags; }
   void setFlags(int flags) { mFlags = flags; }

   Access getVisibility() const { return mAccess; }
   void setVisibility(Access access) { mAccess = access; }
   
private:
   Access mAccess;
   int    mFlags;
};

#endif // AST_MODIFIERS_H_
