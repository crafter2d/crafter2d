#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <cassert>

#include "entitycomponent.h"

class PhysicsComponent : public EntityComponent
{
public:
    enum ShapeType { eBox, eCircle, eInvalid };
    enum Flags { eFixedRotation = 1, eStatic = 2 };

    PhysicsComponent();

    bool isStatic() const {
        return (mFlags & eStatic) == eStatic;
    }

    void setStatic(bool isstatic) {
        if ( isstatic )
        {
            mFlags |= eStatic;
        }
        else
        {
            mFlags &= ~eStatic;
        }
    }

    bool isFixedRotation() const {
        return (mFlags & eFixedRotation) == eFixedRotation;
    }

    void setFixedRotation(bool fixed) {
        if ( fixed )
        {
            mFlags |= eFixedRotation;
        }
        else
        {
            mFlags &= ~eFixedRotation;
        }
    }

    float getMass() const {
        return mMass;
    }

    void setMass(float mass) {
        mMass = mass;
    }

    ShapeType getShape() const {
        return mType;
    }

    void setShape(ShapeType type) {
        mType = type;
    }

    float getWidth() const {
        return mInfoA;
    }

    void setWidth(float width) {
        assert(mType == eBox);
        mInfoA = width;
    }

    float getHeight() const {
        return mInfoB;
    }

    void setHeight(float height) {
        assert(mType == eBox);
        mInfoB = height;
    }

    float getRadius() const {
        return mInfoA;
    }

    void setRadius(float radius) {
        assert(mType == eCircle);
        mInfoA = radius;
    }

private:

    ShapeType mType;
    float mInfoA;       // means for box: width & circle: radius
    float mInfoB;       // only box height
    float mMass;
    int mFlags;
};

#endif // PHYSICSCOMPONENT_H
