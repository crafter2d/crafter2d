#include "spritecomponent.h"

#include "entitycomponentregistry.h"

const QString SpriteComponent::sComponentName("SpriteComponent");
ComponentId SpriteComponent::sComponentId(EntityComponentRegistry::getInstance().add<SpriteComponent>(EntityComponent::GetIdFromString(sComponentName)));

SpriteComponent::SpriteComponent():
    mSize(),
    mAnimations(),
    mAnimSpeed(0)
{

}

// - Get/set

void SpriteComponent::addAnimation(SpriteAnimation&& that)
{
    mAnimations.append(that);
}

// query

ComponentId SpriteComponent::componentId() const
{
    return sComponentId;
}
