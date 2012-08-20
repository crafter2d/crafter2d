
#ifndef COLLECTABLE_H
#define COLLECTABLE_H

class VirtualMachine;

class Collectable
{
public:
            Collectable();
   virtual ~Collectable() = 0;

 // get/set
   bool isMarked() const;
   void setMarked(bool marked);

 // marking
   void mark();

 // finalizing
   virtual void finalize(VirtualMachine& machine) = 0;

protected:
   virtual void doMark() = 0;

private:
   bool mMarked;
};

#endif // COLLECTABLE_H
