#include "entity.h"

Entity::Entity():
    Resource(eObject)
{

}

void Entity::addComponent(EntityComponent* pcomponent)
{
    mComponents.append(pcomponent);
}
