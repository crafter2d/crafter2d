#include "tilefield.h"

#include <QByteArray>
#include <QDataStream>

QTileField* QTileField::fromByteArray(const QSize& dimension, QByteArray &data)
{
    QTileField* presult = new QTileField();
    presult->create(dimension, (quint8*)data.constData());
    return presult;
}

QTileField::QTileField():
    mpField(nullptr),
    mDimension()
{
}

QTileField::~QTileField()
{
    delete mpField;
    mpField = nullptr;
}

// - Get/set

quint8 QTileField::get(Level level, int x, int y) const
{
    Q_ASSERT(x >= 0 && y >= 0 && x < mDimension.width() && y < mDimension.height());
    int index = tileindex(level, x, y);
    return mpField[index];
}

void QTileField::set(Level level, int x, int y, quint8 tile)
{
    Q_ASSERT( x >= 0 && y >= 0 && x < mDimension.width() && y < mDimension.height());
    int index = tileindex(level, x, y);
    mpField[index] = tile;
}

// - Conversion

QByteArray QTileField::toByteArray() const
{
    return QByteArray((const char*)mpField, mDimension.width() * mDimension.height() * 3);
}

// - Operations

void QTileField::create(const QSize& dimension, const quint8* pdata)
{
    mDimension = dimension;
    int size = dimension.width() * dimension.height() * 3;
    mpField = new quint8[size];
    if ( pdata == NULL )
    {
        memset(mpField, 255, sizeof(quint8) * size);
    }
    else
    {
        memcpy(mpField, pdata, sizeof(quint8) * size);
    }
}

void QTileField::resize(const QSize& newdim)
{
    int datasize = newdim.width() * newdim.height() * 3;
    quint8* pfield = new quint8[datasize];
    memset(pfield, -1, sizeof(quint8) * datasize);

    int minwidth = qMin(newdim.width(), mDimension.width());
    int minheight = qMin(newdim.height(), mDimension.height());

    int srcsize = sizeof(quint8) * mDimension.width() * 3;
    int destsize = sizeof(quint8) * newdim.width() * 3;

    quint8* psrc = pfield;
    quint8* pdest = mpField;

    for ( int y = 0; y < minheight; ++y )
    {
      memcpy(psrc, pdest, sizeof(quint8) * minwidth * 3);
      psrc += srcsize;
      pdest += destsize;
    }

    delete[] mpField;
    mpField = pfield;
    mDimension = newdim;
}

// - Streaming

QDataStream& operator<<(QDataStream& stream, const QTileField& field)
{
    QByteArray array = field.toByteArray();
    QByteArray compressed = qCompress(array);
    stream << field.mDimension << compressed;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, QTileField& field)
{
    QSize dimension;
    QByteArray compressed;
    stream >> dimension >> compressed;
    QByteArray data = qUncompress(compressed);
    field.create(dimension, (quint8*)data.constData());
    return stream;
}
