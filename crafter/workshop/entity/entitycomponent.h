#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

class QString;

using ComponentId = unsigned int;

class EntityComponent
{
public:
    static ComponentId GetIdFromString(const QString& name);

    EntityComponent();

    virtual ComponentId componentId() const = 0;

private:

};

#endif // ENTITYCOMPONENT_H
