#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <QString>
#include <QVector>

class SpriteAnimationTile
{
public:

    SpriteAnimationTile();
    explicit SpriteAnimationTile(const QString& tilename);

    const QString& getName() const {
        return mName;
    }

    void setName(const QString& name) {
        mName = name;
    }

private:

    QString mName;
};


class SpriteAnimation
{
public:
    using Tiles = QVector<SpriteAnimationTile>;

    SpriteAnimation();
    SpriteAnimation(const SpriteAnimation& that);
    SpriteAnimation(SpriteAnimation&& that);

    SpriteAnimation& operator=(const SpriteAnimation& that);
    SpriteAnimation& operator=(SpriteAnimation&& that);

    const QString& getName() const {
        return mName;
    }

    void setName(const QString& name) {
        mName = name;
    }

    void addTile(const QString& tilename);

private:

    QString mName;
    Tiles   mTiles;
};

#endif // SPRITEANIMATION_H
