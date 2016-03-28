#include "entityreader.h"

#include <memory>

#include <QSize>

#include "entity.h"
#include "spritecomponent.h"
#include "spriteanimation.h"
#include "physicscomponent.h"

EntityReader::EntityReader(QIODevice &device):
    mDoc()
{
    mDoc.setContent(&device);
}

// - Loading

Entity* EntityReader::read()
{
    QDomElement element = mDoc.firstChildElement("entity");
    if ( !element.isNull() )
    {
        return readEntity(element);
    }

    return nullptr;
}

Entity* EntityReader::readEntity(QDomElement& element)
{
    auto entity = std::make_unique<Entity>();

    if ( element.hasAttribute("name") )
    {
        QString name = element.attribute("name");
        if ( name.isNull() || name.isEmpty() )
        {
            throw new std::runtime_error("Empty name is not allowed.");
        }
        entity->setName(name);
    }
    else
    {
        //mReader.raiseError(QObject::tr("Missing required name attribute."));
        return nullptr;
    }

    if ( element.hasAttribute("class") )
    {
        QString klass = element.attribute("class");
        if ( !klass.isEmpty() )
        {
            entity->setClass(klass);
        }
    }

    QDomElement childelement = element.firstChildElement();
    while ( !childelement.isNull()  )
    {
        if ( childelement.tagName() == "sprite" )
        {
            auto psprite = readSpriteComponent(childelement);
            entity->addComponent(psprite);
        }
        else if ( childelement.tagName() == "body" )
        {
            auto pphysics = readPhysicsComponent(childelement);
            entity->addComponent(pphysics);
        }

        childelement = childelement.nextSiblingElement();
    }

    return entity.release();
}

SpriteComponent* EntityReader::readSpriteComponent(QDomElement &element)
{
    auto sprite = std::make_unique<SpriteComponent>();

    QSize size;
    QString width = element.attribute("width");
    size.setWidth(width.toInt());

    QString height = element.attribute("height");
    size.setHeight(height.toInt());

    sprite->setSize(size);

    QDomElement animationelement = element.firstChildElement("animations");
    if ( !animationelement.isNull() )
    {
        QString speed = animationelement.attribute("speed", "100");
        sprite->setAnimationSpeed(speed.toInt());

        QDomElement animelement = animationelement.firstChildElement("anim");
        while ( !animelement.isNull() )
        {
            SpriteAnimation animation;

            if ( animelement.hasAttribute("name") )
            {
                animation.setName(animelement.attribute("name"));
            }

            QDomElement tileelement = animelement.firstChildElement("tile");
            while ( !tileelement.isNull() )
            {
                QString tilename = animelement.attribute("name");
                if ( tilename.isNull() || tilename.isEmpty() )
                {
                    // invalid name!
                }

                animation.addTile(tilename);
                tileelement = tileelement.nextSiblingElement("tile");
            }

            sprite->addAnimation(std::move(animation));

            animelement = animelement.nextSiblingElement("anim");
        }
    }

    return sprite.release();
}

PhysicsComponent* EntityReader::readPhysicsComponent(QDomElement &element)
{
    auto physics = std::make_unique<PhysicsComponent>();

    if ( element.hasAttribute("static") )
    {
        physics->setStatic(element.attribute("static").toInt() != 0);
    }

    if ( element.hasAttribute("rotate") )
    {
        physics->setFixedRotation(element.attribute("rotate").toInt() != 0);
    }

    if ( element.hasAttribute("mass") )
    {
        physics->setMass(element.attribute("mass").toFloat());
    }

    QDomElement shapeelement = element.firstChildElement("shape");
    if ( !shapeelement.isNull() )
    {
        if ( shapeelement.hasAttribute("type") )
        {
            QString shapetype = shapeelement.attribute("type");
            if ( shapetype == "box" )
            {
                physics->setShape(PhysicsComponent::eBox);

                float width = shapeelement.attribute("halfx").toFloat();
                physics->setWidth(width);

                float height = shapeelement.attribute("halfy").toFloat();
                physics->setHeight(height);
            }
            else if ( shapetype == "circle" )
            {
                physics->setShape(PhysicsComponent::eCircle);

                float radius = shapeelement.attribute("radius").toFloat();
                physics->setRadius(radius);
            }
        }
    }
    else
    {
        // report error, missing required shape element!
    }

    return physics.release();
}

