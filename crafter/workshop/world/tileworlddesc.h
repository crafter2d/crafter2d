#ifndef TILEWORLDDESC_H
#define TILEWORLDDESC_H

#include <QtGlobal>
#include <QString>

class QDataStream;

struct TileWorldDesc
{
    QString name;
    QString simulator;
    uint    version;
    uint    layerType;
};

QDataStream& operator<<(QDataStream& stream, const TileWorldDesc& desc);
QDataStream& operator>>(QDataStream& stream, TileWorldDesc& desc);

#endif // TILEWORLDDESC_H
