#ifndef ENTITY_H
#define ENTITY_H

#include <QVector>

#include "../resource.h"

class EntityComponent;

class Entity : public Resource
{
    using Components = QVector<EntityComponent*>;

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

private:

  // data
    QString mName;
    QString mClass;
    Components mComponents;
};

#endif // ENTITY_H
