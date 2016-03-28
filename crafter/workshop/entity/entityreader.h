#ifndef ENTITYREADER_H
#define ENTITYREADER_H

#include <QXmlStreamReader>
#include <QDomDocument>
#include <QDomElement>

class Entity;
class PhysicsComponent;
class SpriteComponent;

class EntityReader
{
public:
    explicit EntityReader(QIODevice &device);

    Entity* read();

private:

    Entity*             readEntity(QDomElement &element);
    SpriteComponent*    readSpriteComponent(QDomElement &element);
    PhysicsComponent*   readPhysicsComponent(QDomElement &element);

  // data
    QDomDocument mDoc;
};

#endif // ENTITYREADER_H
