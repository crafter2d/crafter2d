#include "spritesheetwriter.h"

#include <QDir>
#include <QFile>
#include <QXmlStreamWriter>
#include <QVariant>

#include "spriteatlas.h"
#include "spritesheet.h"

static const QString sAtlasFilename("tileatlas.xml");
static const QString sSheetFilename("tilesheet_%1");

const QString& SpriteAtlasWriter::atlasFilename()
{
    return sAtlasFilename;
}

void SpriteAtlasWriter::write(const QString& path, const SpriteAtlas& atlas)
{
    SpriteAtlasWriter writer(path);
    writer.writeAtlas(atlas);
}

SpriteAtlasWriter::SpriteAtlasWriter(const QString& path):
    mPath(path),
    mSheetIndex(1)
{
}

void SpriteAtlasWriter::writeAtlas(const SpriteAtlas& atlas)
{
    mSheetIndex = 1;
    QFile file(mPath + QDir::separator() + sAtlasFilename);

    if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();

        writer.writeStartElement("tileatlas");

        for ( auto& sheet : atlas.getSheets() )
        {
            writeSheet(writer, sheet);
        }

        writer.writeEndElement(); // tileatlas

        writer.writeEndDocument();
    }
}

void SpriteAtlasWriter::writeSheet(QXmlStreamWriter& writer, const SpriteSheet& sheet)
{
    QString imageFilename = sSheetFilename.arg(mSheetIndex++);

    writer.writeStartElement("tilesheet");
    writer.writeAttribute("name", "tileatlas/" + imageFilename);
    writer.writeAttribute("width", QString::number(sheet.image().width()));
    writer.writeAttribute("height", QString::number(sheet.image().height()));

    for ( const Sprite& sprite : sheet.sprites() )
    {
        writer.writeStartElement("tile");
        writer.writeAttribute("name", sprite.mName);
        writer.writeAttribute("x", QString::number(sprite.mCoordinates.x()));
        writer.writeAttribute("y", QString::number(sprite.mCoordinates.y()));
        writer.writeAttribute("w", QString::number(sprite.mCoordinates.width()));
        writer.writeAttribute("h", QString::number(sprite.mCoordinates.height()));
        if ( sprite.mRotated )
        {
            writer.writeAttribute("r", "true");
        }

        writer.writeEndElement();
    }
    writer.writeEndElement();

    sheet.save(mPath + QDir::separator() + imageFilename + ".png");
}
