#ifndef TILEMAPMODEL_H
#define TILEMAPMODEL_H

#include <QAbstractTableModel>

class TileWorld;

class TileMapModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    TileMapModel();

  // get/set
    bool        hasWorld() const;
    TileWorld*  getWorld();
    void        setWorld(TileWorld *pworld);
    void        clearWorld();

  // operations
    void moveUp(const QModelIndex &index);
    void moveDown(const QModelIndex& index);

  // overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

private:

  // data
    TileWorld* mpWorld;
};

#endif // TILEMAPMODEL_H
