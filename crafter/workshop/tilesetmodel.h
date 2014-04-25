#ifndef TILESETMODEL_H
#define TILESETMODEL_H

#include <QAbstractTableModel>

class Project;

class TileSetModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TileSetModel(QObject *parent = 0);

  // overrides
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

signals:

public slots:

private slots:
    void on_projectChanged(Project* pnewproject);

private:

  // data
    Project* mpProject;

};

#endif // TILESETMODEL_H
