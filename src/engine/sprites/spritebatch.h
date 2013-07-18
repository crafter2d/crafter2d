
#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include <vector>

class Sprite;

class SpriteBatch
{
public:

   const Sprite& operator[](int index) const;

 // query
   int size() const;

   void add(const Sprite& sprite);
   void clear();

   void sort();

private:
   typedef std::vector<const Sprite*> Sprites;

 // data
   Sprites mSprites;
};

#endif // SPRITE_BATCH_H
