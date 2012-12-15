

#include "entities.h"

#include "entity.h"
#include "nodevisitor.h"

Entities::Entities():
   mEntities()
{
}

Entities::~Entities()
{
}

// - Maintenance

void Entities::add(Entity& entity)
{
   mEntities[entity.getId()] = &entity;
}

void Entities::remove(Entity& entity)
{
   EntityMap::iterator it = mEntities.find(entity.getId());
   ASSERT(it != mEntities.end());
   mEntities.erase(it);
}

// - update & drawing

void Entities::update(float delta)
{
   EntityMap::iterator it = mEntities.begin();
   for ( ; it != mEntities.end(); ++it )
   {
      Entity* pentity = it->second;
      pentity->update(delta);
   }
}

void Entities::draw(Graphics::RenderContext& context) const
{
   EntityMap::const_iterator it = mEntities.begin();
   for ( ; it != mEntities.end(); ++it )
   {
      Entity* pentity = it->second;
      pentity->draw(context);
   }
}

// - Visitor
   
void Entities::traverse(NodeVisitor& visitor)
{
   EntityMap::const_iterator it = mEntities.begin();
   for ( ; it != mEntities.end(); ++it )
   {
      Entity* pentity = it->second;
      visitor.traverse(*pentity);
   }
}
