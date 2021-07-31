#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include <QObject>

class QString;

using ComponentId = unsigned int;

class EntityComponent: public QObject
{
    Q_OBJECT

public:
    static ComponentId GetIdFromString(const QString& name);

    EntityComponent();

    virtual ComponentId componentId() const = 0;

private:

};

#endif // ENTITYCOMPONENT_H
