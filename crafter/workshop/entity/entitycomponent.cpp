#include "entitycomponent.h"

#include <QString>
#include <QHash>

// static
ComponentId EntityComponent::GetIdFromString(const QString& name)
{
    return static_cast<ComponentId>(qHash(name));
}

EntityComponent::EntityComponent()
{

}
