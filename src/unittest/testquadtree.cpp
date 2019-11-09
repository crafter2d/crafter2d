
#include <gtest/gtest.h>

#include "core/math/rect.h"
#include "core/quadtree/quadtree.h"
#include "core/graphics/sprites/sprite.h"

class QuadTreeFixture : public ::testing::Test
{
protected:

   c2d::Sprite createSprite(const Vector& pos) {
      auto def = std::make_unique<c2d::SpriteDefinition>();
      def->setSize(Size(10, 10));
      c2d::Sprite sprite(def.release());
      sprite.setTransform(XForm(pos, 0));
      sprite.initialize();
      return sprite;
   }

};

TEST_F(QuadTreeFixture, NewQuadTreeIsEmpty)
{
   c2d::QuadTree tree(Size(10, 10));
   EXPECT_TRUE(tree.empty());
}

TEST_F(QuadTreeFixture, InsertSprite)
{
   c2d::Sprite sprite = createSprite(Vector(0, 0));
   c2d::QuadTree tree(Size(100, 100));
   tree.insert(sprite);
   EXPECT_FALSE(tree.empty());
}

TEST_F(QuadTreeFixture, LookupSprites)
{
   c2d::QuadTree tree(Size(200, 200));
   Vector positions[] = { {5, 5}, {15, 15}, {35,35}, {75,75}, 
                          {115,115}, {175,175}, {185,185} };
   std::vector<c2d::Sprite> sprites;
   for ( auto& pos : positions )
   {
      sprites.push_back(createSprite(pos));
   }

   for ( auto& sprite : sprites )
   {
      tree.insert(sprite);
   }

   EXPECT_EQ(tree.count(), 7); // make sure we have saved all sprites in the tree

   c2d::RectF bounds(0, 0, 100, 100);
   auto visible_sprites = tree.get(bounds);
   EXPECT_FALSE(visible_sprites.empty());
   EXPECT_TRUE(visible_sprites.size() == 4);

   bounds = c2d::RectF(100, 100, 200, 200);
   visible_sprites = tree.get(bounds);
   EXPECT_FALSE(visible_sprites.empty());
   EXPECT_EQ(3, visible_sprites.size());

   bounds = c2d::RectF(200, 200, 50, 50);
   visible_sprites = tree.get(bounds);
   EXPECT_TRUE(visible_sprites.empty());
}
