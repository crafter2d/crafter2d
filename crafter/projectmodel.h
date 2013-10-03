#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractTableModel>

class Project;

class ProjectModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ProjectModel();

 // get/set
    void setProject(Project* pproject);

 // overloads
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

private slots:
    void onDataChanged();

private:

    Project* mpProject;
};

#endif // PROJECTMODEL_H
