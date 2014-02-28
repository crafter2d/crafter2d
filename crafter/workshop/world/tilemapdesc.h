#ifndef TILEMAPDESC_H
#define TILEMAPDESC_H

#include <QSize>
#include <QString>

struct TileMapDesc
{
    QString name;
    QString effect;
    QString tileset;
    QSize   size;
};

QDataStream& operator<<(QDataStream& stream, const TileMapDesc& desc);
QDataStream& operator>>(QDataStream& stream, TileMapDesc& desc);

#endif // TILEMAPDESC_H
