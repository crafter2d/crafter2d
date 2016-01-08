#include "tilesetmodel.h"

#include "project/projectmanager.h"
#include "project.h"
#include "world/tileset.h"

TileSetModel::TileSetModel(QObject *parent) :
    QAbstractTableModel(parent),
    mpProject(nullptr)
{
    ProjectManager& mgr = ProjectManager::getInstance();
    mpProject = mgr.getActiveProject();

    connect(&mgr, SIGNAL(projectChanged(Project*)), SLOT(on_projectChanged(Project*)));
}

// - Overrides

int TileSetModel::rowCount(const QModelIndex &/*parent*/) const
{
    if ( mpProject != NULL )
    {
        return mpProject->getTileSets().size();
    }
    return 0;
}

int TileSetModel::columnCount(const QModelIndex& /* parent */) const
{
    return 1;
}

QVariant TileSetModel::headerData(int section, Qt::Orientation /* orientation */, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
        case 0: return tr("Name");
    }

    return QVariant();
}

QVariant TileSetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

    switch ( index.column() )
    {
    case 0:
        {
            TileSet* ptileset = mpProject->getTileSets()[index.row()];
            QString name = ptileset->getResourceName();
            if ( name.isEmpty() )
            {
                name = tr("<no name>");
            }
            return QVariant(name);
        }
    }

    return QVariant();
}

// - Slots

void TileSetModel::on_projectChanged(Project* pnewproject)
{
    beginResetModel();
    mpProject = pnewproject;
    endResetModel();
}
