#ifndef TILEFIELD_H
#define TILEFIELD_H

#include <QtGlobal>
#include <QSize>

class QByteArray;

class QTileField
{
public:
    enum Level { eFront, eMid, eBack };

    static QTileField* fromByteArray(const QSize &dimension, QByteArray& data);

    QTileField();
    ~QTileField();

  // operations
    quint8 get(Level level, int x, int y) const;
    void   set(Level level, int x, int y, quint8 tile);

  // operations
    void create(const QSize& dimension, const quint8 *pdata = NULL);
    void resize(const QSize& newdim);

  // conversion
    QByteArray QTileField::toByteArray() const;

  // streaming
    friend QDataStream& operator<<(QDataStream& stream, const QTileField& field);
    friend QDataStream& operator>>(QDataStream& stream, QTileField& field);

private:

  // helpers
    int tileindex(Level level, int x, int y) const;

  // data
    quint8* mpField;
    QSize   mDimension;
};

inline int QTileField::tileindex(Level level, int x, int y) const
{
   return (y * mDimension.width() + x) * 3 + (quint8)level;
}

#endif // TILEFIELD_H
