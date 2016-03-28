#include "entitycomponentregistry.h"

// static
EntityComponentRegistry& EntityComponentRegistry::getInstance()
{
    static EntityComponentRegistry reg;
    return reg;
}

EntityComponentRegistry::EntityComponentRegistry()
{

}
