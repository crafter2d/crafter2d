#ifndef TILEFIELD_H
#define TILEFIELD_H

#include <QtGlobal>
#include <QSize>

class QByteArray;

class TileField final
{
public:
    enum Level { eFront, eMid, eBack };

    static TileField* fromByteArray(const QSize &dimension, QByteArray& data);

    TileField();
    explicit TileField(const QSize& dimension);
    ~TileField();

  // operations
    quint8 get(Level level, int x, int y) const;
    void   set(Level level, int x, int y, quint8 tile);

  // operations
    void create(const QSize& dimension, const quint8 *pdata = nullptr);
    void resize(const QSize& newdim);

  // conversion
    QByteArray toByteArray() const;

  // streaming
    friend QDataStream& operator<<(QDataStream& stream, const TileField& field);
    friend QDataStream& operator>>(QDataStream& stream, TileField& field);

private:

  // helpers
    int tileindex(Level level, int x, int y) const;

  // data
    quint8* mpField;
    QSize   mDimension;
};

inline int TileField::tileindex(Level level, int x, int y) const
{
   return (y * mDimension.width() + x) * 3 + (quint8)level;
}

#endif // TILEFIELD_H
