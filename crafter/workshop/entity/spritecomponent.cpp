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
    mAnimations.push_back(std::move(that));
    emit dataChanged();
}

void SpriteComponent::clearAnimations(){
    mAnimations.clear();
    emit dataChanged();
}

// query

ComponentId SpriteComponent::componentId() const
{
    return sComponentId;
}
