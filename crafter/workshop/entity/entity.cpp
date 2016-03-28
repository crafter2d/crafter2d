#include "entity.h"

Entity::Entity():
    Resource(eObject)
{

}

void Entity::addComponent(EntityComponent* pcomponent)
{
    auto it = mComponents.find(pcomponent->componentId());
    if (it != mComponents.end())
    {
     // we already have such a component!
     return;
    }

    mComponents[pcomponent->componentId()] = pcomponent;
    //pcomponent->SetOwner(*this);
}
