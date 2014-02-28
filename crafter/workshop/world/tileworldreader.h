#ifndef TILEWORLDREADER_H
#define TILEWORLDREADER_H

#include <QString>

class QIODevice;

class TileWorld;

class QTileWorldReader
{
public:
    QTileWorldReader(QIODevice& device);

  // get/set
    const QString& baseDir() const;
    void           setBaseDir(const QString& basedir);

  // reading
    TileWorld *read();

private:

  // data
    QIODevice& mDevice;
    QString    mBaseDir;
};

#endif // TILEWORLDREADER_H
