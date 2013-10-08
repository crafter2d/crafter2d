#include "projectmodel.h"

#include "project.h"
#include "tileworld.h"

ProjectModel::ProjectModel():
    QAbstractTableModel(),
    mpProject()
{
}

// - Get/set

void ProjectModel::setProject(Project* pproject)
{
    if ( mpProject != pproject )
    {
        if ( mpProject != NULL )
        {
            disconnect(mpProject, SIGNAL(dataChanged()));
        }

        beginResetModel();
        mpProject = pproject;
        endResetModel();

        if ( mpProject != NULL )
        {
            connect(mpProject, SIGNAL(dataChanged()), SLOT(onDataChanged()));
        }
    }
}

// - Overloads

int ProjectModel::rowCount(const QModelIndex &/*parent*/) const
{
    return mpProject != NULL ? mpProject->getWorldCount() : 0;
}

int ProjectModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();

    switch (section)
    {
        case 0: return tr("Name");
    }

    return QVariant();
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

    const TileWorld& world = mpProject->getWorld(index.row());
    QString worldname = world.getName();
    if ( worldname.isEmpty() )
    {
        worldname = "<no name>";
    }

    return QVariant(worldname);
}

// - Slots

void ProjectModel::onDataChanged()
{
    beginResetModel();
    endResetModel();
}
