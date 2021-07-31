#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <QObject>
#include <QString>
#include <QVector>

class SpriteAnimationTile: public QObject
{
    Q_OBJECT
public:

    SpriteAnimationTile();
    SpriteAnimationTile(const SpriteAnimationTile& that);
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


class SpriteAnimation: public QObject
{
    Q_OBJECT
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

    Tiles& getTiles() { return mTiles; }
    const Tiles& getTiles() const {
        return mTiles;
    }
    void addTile(const QString& tilename);

private:

    QString mName;
    Tiles   mTiles;
};

Q_DECLARE_METATYPE(SpriteAnimation)
Q_DECLARE_METATYPE(SpriteAnimationTile)

#endif // SPRITEANIMATION_H
