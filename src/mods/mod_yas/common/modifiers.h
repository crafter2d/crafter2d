
#ifndef MODIFIERS_H
#define MODIFIERS_H

class Modifiers
{
public:

   enum Flags { eAbstract = 1, ePureNative = 2, eStatic = 4 };

   Modifiers(int flags = 0);
   Modifiers(const Modifiers& that);

   Modifiers& operator=(const Modifiers& that);

   // get/set
   bool isAbstract() const;
   void setAbstract(bool isabstract);

   bool isPureNative() const;
   void setPureNative(bool ispurenative);

   bool isStatic() const;
   void setStatic(bool isstatic);

   // conversion
   int toInt() const;

private:

   // data
   int mFlags;
};

#endif // MODIFIERS_H
