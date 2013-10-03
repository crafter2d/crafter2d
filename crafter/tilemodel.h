#ifndef TILEMODEL_H
#define TILEMODEL_H

#include <QAbstractListModel>

class TileMap;

class TileModel : public QAbstractListModel
{
    Q_OBJECT

public:
    TileModel(QObject* parent = 0);

  // get/set
    void setMap(TileMap& map);
    void clearMap();

  // overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

private:

  // get/set
    void setMap(TileMap* pmap);

  // data
    TileMap* mpMap;
};

#endif // TILEMODEL_H
