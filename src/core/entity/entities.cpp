

#include "entities.h"

#include <algorithm>

#include "core/defines.h"

#include "entity.h"

Entities::Entities():
   mEntities()
{
}

Entities::~Entities()
{
   removeAll();
}

const Entity& Entities::operator[](int index) const
{
   return *mEntities[index];
}

Entity& Entities::operator[](int index)
{
   return *mEntities[index];
}

// - Maintenance

void Entities::add(Entity* pentity)
{
   mEntities.push_back(pentity);
}

void Entities::remove(Entity& entity)
{
   EntityMap::iterator it = std::find(mEntities.begin(), mEntities.end(), &entity);
   ASSERT(it != mEntities.end());
   mEntities.erase(it);

   delete &entity;
}

void Entities::removeAll()
{
   for ( std::size_t index = 0; index < mEntities.size(); ++index )
   {
      Entity* pentity = mEntities[index];
      delete pentity;
   }
   mEntities.clear();
}

// - update & drawing

void Entities::initialize()
{
   for ( std::size_t index = 0; index < mEntities.size(); ++index )
   {
      Entity* pentity = mEntities[index];
      pentity->initialize();
   }
}

void Entities::update(float delta)
{
   for ( std::size_t index = 0; index < mEntities.size(); ++index )
   {
      Entity* pentity = mEntities[index];
      pentity->update(delta);
   }
}

void Entities::draw(Graphics::RenderContext& context) const
{
   for ( std::size_t index = 0; index < mEntities.size(); ++index )
   {
      Entity* pentity = mEntities[index];
      pentity->draw(context);
   }
}

void Entities::setPosition(const Vector& pos)
{
   for ( std::size_t index = 0; index < mEntities.size(); ++index )
   {
      Entity* pentity = mEntities[index];
      pentity->setPosition(pos);
   }
}


/*
// - Visitor
   
void Entities::traverse(NodeVisitor& visitor)
{
   for ( std::size_t index = 0; index < mEntities.size(); ++index )
   {
      Entity* pentity = mEntities[index];
      visitor.traverse(*pentity);
   }
}
*/