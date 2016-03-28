#ifndef ENTITY_H
#define ENTITY_H

#include <QMap>

#include "../resource.h"

#include "entitycomponent.h"

class Entity : public Resource
{
    using Components = QMap<ComponentId, EntityComponent*>;

public:
    Entity();

  // get/set
    const QString& getName() const {
        return mName;
    }

    void setName(const QString& name) {
        mName = name;
    }

    const QString& getClass() const {
        return mClass;
    }

    void setClass(const QString& klass) {
        mClass = klass;
    }

    void addComponent(EntityComponent* pcomponent);

    template <class ComponentType>
    ComponentType* component()
    {
        auto it = mComponents.find(ComponentType::sComponentId);
        if (it != mComponents.end())
        {
            return static_cast<ComponentType*>(*it);
        }

        return nullptr;
    }

private:

  // data
    QString mName;
    QString mClass;
    Components mComponents;
};

#endif // ENTITY_H
