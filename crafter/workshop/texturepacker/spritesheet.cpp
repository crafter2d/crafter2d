#include "spritesheet.h"

#include <QPainter>
#include <QWidget>

#include "../helpers/rotateimagepainter.h"

#include "spritesheetreader.h"
#include "spritesheetwriter.h"

SpriteSheet::SpriteSheet():
    mImage(),
    mSprites(),
    mSpriteIndex()
{
}

SpriteSheet::SpriteSheet(const SpriteSheet& that):
    mImage(that.mImage),
    mSprites(that.mSprites),
    mSpriteIndex(that.mSpriteIndex)
{
}

#ifdef Q_COMPILER_RVALUE_REFS

SpriteSheet::SpriteSheet(SpriteSheet&& that):
    mImage(qMove(that.mImage)),
    mSprites(qMove(that.mSprites)),
    mSpriteIndex(qMove(that.mSpriteIndex))
{
}

SpriteSheet& SpriteSheet::operator=(SpriteSheet&& that)
{
    mImage = qMove(that.mImage);
    mSprites = qMove(that.mSprites);
    mSpriteIndex = qMove(that.mSpriteIndex);
    return *this;
}

#endif

int SpriteSheet::lookup(const QString& spritename) const
{
    auto it = mSpriteIndex.find(spritename);
    return it != mSpriteIndex.end() ? *it : INVALID_INDEX;
}

QRect SpriteSheet::bounds(int index)
{
    return mSprites[index].mCoordinates;
}

QImage& SpriteSheet::newImage(int width, int height)
{
    mImage = qMove(QImage(width, height, QImage::Format_ARGB32));
    mImage.fill(Qt::transparent);
    return mImage;
}

void SpriteSheet::addSprite(const Sprite& sprite)
{
    mSprites.append(sprite);
    mSpriteIndex.insert(sprite.mName, mSprites.size() - 1);
}

void SpriteSheet::clear()
{
    mSprites.clear();
    mSpriteIndex.clear();
}

void SpriteSheet::paint(QPainter& painter, const QPoint& pos, int index)
{
    const Sprite& sprite = mSprites[index];
    if ( sprite.mRotated )
    {
        RotateImagePainter rpainter(painter);
        rpainter.drawImage(pos,
                           -90,
                           sprite.mCoordinates.width(),
                           sprite.mCoordinates.height(),
                           sprite.mCoordinates.height(),
                           sprite.mCoordinates.width(),
                           mImage,
                           sprite.mCoordinates);
    }
    else
    {
        const QRect& coords = mSprites[index].mCoordinates;
        painter.drawImage(pos, mImage, coords);
    }
}

bool SpriteSheet::load(const QString& filename)
{
    return mImage.load(filename);
}

bool SpriteSheet::save(const QString& filename) const
{
    return mImage.save(filename, "PNG");
}
