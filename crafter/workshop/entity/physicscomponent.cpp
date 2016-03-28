#include "physicscomponent.h"

#include "entitycomponentregistry.h"

const QString PhysicsComponent::sComponentName("PhysicsComponent");
ComponentId PhysicsComponent::sComponentId(EntityComponentRegistry::getInstance().add<PhysicsComponent>(EntityComponent::GetIdFromString(sComponentName)));

PhysicsComponent::PhysicsComponent():
    mType(eInvalid),
    mInfoA(0.0f),
    mInfoB(0.0f),
    mMass(0.0f),
    mFlags()
{

}

ComponentId PhysicsComponent::componentId() const
{
    return sComponentId;
}

