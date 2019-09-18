#include "tilefield.h"

#include <QByteArray>
#include <QDataStream>

TileField* TileField::fromByteArray(const QSize& dimension, QByteArray &data)
{
    TileField* presult = new TileField();
    presult->create(dimension, reinterpret_cast<const quint8*>(data.constData()));
    return presult;
}

TileField::TileField():
    mpField(nullptr),
    mDimension()
{
}

TileField::TileField(const QSize& dimension):
    mpField(nullptr),
    mDimension()
{
    create(dimension);
}

TileField::~TileField()
{
    delete mpField;
    mpField = nullptr;
}

// - Get/set

quint8 TileField::get(Level level, int x, int y) const
{
    Q_ASSERT(x >= 0 && y >= 0 && x < mDimension.width() && y < mDimension.height());
    int index = tileindex(level, x, y);
    return mpField[index];
}

void TileField::set(Level level, int x, int y, quint8 tile)
{
    Q_ASSERT( x >= 0 && y >= 0 && x < mDimension.width() && y < mDimension.height());
    int index = tileindex(level, x, y);
    mpField[index] = tile;
}

// - Conversion

QByteArray TileField::toByteArray() const
{
    return QByteArray((const char*)mpField, mDimension.width() * mDimension.height() * 3);
}

// - Operations

void TileField::create(const QSize& dimension, const quint8* pdata)
{
    mDimension = dimension;
    quint32 size = dimension.width() * dimension.height() * 3u;
    mpField = new quint8[size];
    if ( pdata == nullptr )
    {
        memset(mpField, 255, sizeof(quint8) * size);
    }
    else
    {
        memcpy(mpField, pdata, sizeof(quint8) * size);
    }
}

void TileField::resize(const QSize& newdim)
{
    int datasize = newdim.width() * newdim.height() * 3;
    quint8* pfield = new quint8[datasize];
    memset(pfield, 255, sizeof(quint8) * datasize);

    int srcsize = sizeof(quint8) * mDimension.width() * 3;
    int destsize = sizeof(quint8) * newdim.width() * 3;

    int minwidth = qMin(srcsize, destsize);
    int minheight = qMin(mDimension.height(), newdim.height());

    quint8* psrc = mpField;
    quint8* pdest = pfield;

    for ( int y = 0; y < minheight; ++y )
    {
      memcpy(pdest, psrc, minwidth);
      psrc += srcsize;
      pdest += destsize;
    }

    delete[] mpField;
    mpField = pfield;
    mDimension = newdim;
}

// - Streaming

QDataStream& operator<<(QDataStream& stream, const TileField& field)
{
    QByteArray array = field.toByteArray();
    QByteArray compressed = qCompress(array);
    stream << field.mDimension << compressed;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, TileField& field)
{
    QSize dimension;
    QByteArray compressed;
    stream >> dimension >> compressed;
    QByteArray data = qUncompress(compressed);
    field.create(dimension, (quint8*)data.constData());
    return stream;
}
