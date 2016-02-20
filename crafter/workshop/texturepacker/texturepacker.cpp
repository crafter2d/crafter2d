#include "texturepacker.h"
#include "ui_texturepacker.h"

#include <QDir>
#include <QImageReader>
#include <QMessageBox>
#include <QPainter>
#include <QVector>
#include <cassert>

#include "../helpers/rotateimagepainter.h"
#include "../project.h"

#include "maxrectsalgorithm.h"
#include "sprite.h"
#include "spritesheet.h"

TexturePacker::TexturePacker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TexturePacker)
{
    ui->setupUi(this);
}

TexturePacker::~TexturePacker()
{
    delete ui;
}

// - Painting

void TexturePacker::paintEvent(QPaintEvent* /* event */)
{

}

// - Packing

void TexturePacker::pack()
{
    QString basepath = Project::getActiveProject().getImagePath();

    SpriteArray sprites, usedSprites;
    recurseDirectory(sprites, basepath);

    SpriteAtlas& atlas = Project::getActiveProject().getSpriteAtlas();
    atlas.clear();

    do
    {
        MaxRectsAlgorithm maxrects(2046, 2046);
        maxrects.insert(sprites, usedSprites);

        if ( !usedSprites.isEmpty() )
        {
            SpriteSheet sheet = createSpriteSheet(usedSprites);
            atlas.append(qMove(sheet));

            usedSprites.clear();
        }
    }
    while ( !sprites.isEmpty() );

    atlas.save(Project::getActiveProject().getTileAtlasPath());
}

void TexturePacker::recurseDirectory(SpriteArray& sprites, const QString& path)
{
    QStringList filter;
    filter << "png" << "jpg" << "bmp";

    QDir dir(path);
    QFileInfoList infos = dir.entryInfoList();
    for ( auto& info : infos )
    {
        if ( info.isDir() )
        {
            if ( info.fileName() != ".." && info.fileName() != "." )
            {
                recurseDirectory(sprites, info.filePath());
            }
        }
        else if ( filter.contains(info.suffix()) )
        {            
            QImageReader image(info.filePath());

            Sprite sprite;
            sprite.mName = info.filePath();
            sprite.mOriginalSize = image.size();

            sprites.append(sprite);
        }
    }
}

SpriteSheet TexturePacker::createSpriteSheet(SpriteArray& sprites)
{
    SpriteSheet sheet;

    QImage& image = sheet.newImage(2048, 2048);
    QPainter painter(&image);
    QPoint border(1, 1);

    const int baselength = Project::getActiveProject().getImagePath().length() + 1;

    for ( const Sprite& sprite : sprites )
    {
        QImage image(sprite.mName);
        assert(!image.isNull());

        Sprite newSprite;
        newSprite.mName = sprite.mName.right(sprite.mName.length() - baselength);
        newSprite.mCoordinates = sprite.mCoordinates;
        newSprite.mOriginalSize = sprite.mOriginalSize;
        newSprite.mRotated = sprite.mRotated;
        newSprite.translate(border);
        sheet.addSprite(newSprite);

        if ( newSprite.mRotated )
        {
            RotateImagePainter rpainter(painter);
            rpainter.drawImage(newSprite.mCoordinates.topLeft(),
                               90,
                               image.width(),
                               image.height(),
                               newSprite.mCoordinates.width(),
                               newSprite.mCoordinates.height(),
                               image);
        }
        else
        {
            painter.drawImage(newSprite.mCoordinates, image);
        }
    }

    return sheet;
}
