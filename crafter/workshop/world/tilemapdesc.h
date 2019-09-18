#ifndef TILEMAPDESC_H
#define TILEMAPDESC_H

#include <QSize>
#include <QString>

struct TileMapDesc
{
    enum Flags {
        eTileSet = 1,
        eObjects = 2,
    };

    TileMapDesc();

    QString  name;
    QString  effect;
    QString  tileset;
    QSize    size;
    uint32_t flags;
};

QDataStream& operator<<(QDataStream& stream, const TileMapDesc& desc);
QDataStream& operator>>(QDataStream& stream, TileMapDesc& desc);

#endif // TILEMAPDESC_H
