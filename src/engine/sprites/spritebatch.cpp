
#include "spritebatch.h"

#include <algorithm>

#include "sprite.h"

const Sprite& SpriteBatch::operator[](int index) const
{
   ASSERT(index < size());
   return *mSprites[index];
}

// - Query

int SpriteBatch::size() const
{
   return mSprites.size();
}

// - Operations

void SpriteBatch::add(const Sprite& sprite)
{
   mSprites.push_back(&sprite);
}

void SpriteBatch::clear()
{
   mSprites.clear();
}

void SpriteBatch::sort()
{
   std::sort(mSprites.begin(), mSprites.end(), [](const Sprite* pleft, const Sprite* pright)
   {
      return &pleft->getTexture() < &pright->getTexture();
   });
}
