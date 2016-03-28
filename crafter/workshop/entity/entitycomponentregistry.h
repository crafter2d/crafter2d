#ifndef ENTITYCOMPONENTREGISTRY_H
#define ENTITYCOMPONENTREGISTRY_H

#include <QMap>

#include "entitycomponent.h"

class EntityComponentRegistry
{
public:
    static EntityComponentRegistry& getInstance();

    template <class Type>
    ComponentId add(ComponentId id)
    {
        mRegistry[id] = &ConstructFunction<Type>;
        return id;
    }

    template <class ComponentType>
    EntityComponent* construct()
    {
        auto it = mRegistry.find(ComponentType::sComponentId);
        if ( it != mRegistry.end() )
        {
            return (it->second)();
        }
        return nullptr;
    }

private:

    template <class SubType>
    static EntityComponent* ConstructFunction() { return new SubType(); }

    typedef EntityComponent* (*ConstructFunctionPtr)(void);
    using Registry = QMap<ComponentId, ConstructFunctionPtr>;

  // constructs
    EntityComponentRegistry();

  // members
    Registry mRegistry;
};

#endif // ENTITYCOMPONENTREGISTRY_H
