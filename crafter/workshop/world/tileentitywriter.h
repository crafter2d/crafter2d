#ifndef TILEENTITYWRITER_H
#define TILEENTITYWRITER_H

#include "tileworld.h"

class QFile;

class TileEntityWriter
{
public:
    TileEntityWriter();

    void write(QFile& file, TileWorld::Entities& entities);
};

#endif // TILEENTITYWRITER_H
