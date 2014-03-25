#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>

class Project;
class ProjectTreeItem;

class ProjectModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ProjectModel();
    virtual ~ProjectModel();

 // get/set
    void setProject(Project* pproject);

 // overloads
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    QVariant resourceData(const QModelIndex& index);

private slots:
    void onDataChanged();

private:

  // operations
    void buildTree();
    void synchronizeTree();
    void synchronizeWorlds(ProjectTreeItem& parent);

  // data
    Project* mpProject;
    ProjectTreeItem* mpRoot;
};

#endif // PROJECTMODEL_H
