#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <QImage>
#include <QMap>
#include <QVector>

#include "sprite.h"

class SpriteSheet
{
public:
    using SpriteArray = QVector<Sprite>;
    using SpriteMap = QMap<QString, int>;

    static const int INVALID_INDEX = -1;

    SpriteSheet();
    SpriteSheet(const SpriteSheet& that);

#ifdef Q_COMPILER_RVALUE_REFS
    SpriteSheet(SpriteSheet&& that);
    SpriteSheet& operator=(SpriteSheet&& that);
#endif

    int lookup(const QString& spritename) const;
    QRect bounds(int index);

    QImage& newImage(int width, int height);
    const QImage& image() const {
        return mImage;
    }

    const SpriteArray& sprites() const {
        return mSprites;
    }

    void addSprite(const Sprite& sprite);
    void clear();

    void paint(QPainter& painter, const QPoint &pos, int index);

    bool load(const QString& filename);
    bool save(const QString& filename) const;

private:

  // data
    QImage  mImage;
    SpriteArray mSprites;
    SpriteMap mSpriteIndex;
};

#endif // SPRITESHEET_H
