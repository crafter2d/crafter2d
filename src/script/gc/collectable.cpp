
#include "collectable.h" 

Collectable::Collectable():
   mMarked(false)
{
}

Collectable::~Collectable()
{
}

// - Get/set

bool Collectable::isMarked() const
{
   return mMarked;
}

void Collectable::setMarked(bool marked)
{
   mMarked = marked;
}

// - Marking

void Collectable::mark()
{
   if ( !mMarked )
   {
      mMarked = true;
      doMark();
   }
}
