#include "projectmodel.h"

#include "project/projecttreetextitem.h"
#include "project/projecttreeworlditem.h"

#include "project.h"
#include "tileworld.h"

ProjectModel::ProjectModel():
    QAbstractItemModel(),
    mpProject(),
    mpRoot(new ProjectTreeItem())
{
    mpRoot->setRoot();
}

ProjectModel::~ProjectModel()
{
    delete mpRoot;
    mpRoot = NULL;
}

// - Get/set

void ProjectModel::setProject(Project* pproject)
{
    if ( mpProject != pproject )
    {
        beginResetModel();

        if ( mpProject != NULL )
        {
            disconnect(mpProject, SIGNAL(dataChanged()));
        }

        mpProject = pproject;

        if ( mpProject != NULL )
        {
            buildTree();

            connect(mpProject, SIGNAL(dataChanged()), SLOT(onDataChanged()));
        }

        endResetModel();
    }
}

// - Overloads

QModelIndex ProjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    ProjectTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = mpRoot;
    else
        parentItem = static_cast<ProjectTreeItem*>(parent.internalPointer());

    QModelIndex result;
    ProjectTreeItem *childItem = parentItem->child(row);
    if ( childItem != NULL )
        result = createIndex(row, column, childItem);

    return result;
}

QModelIndex ProjectModel::parent(const QModelIndex &index) const
{
    if ( !index.isValid() )
        return QModelIndex();

    ProjectTreeItem *childItem = static_cast<ProjectTreeItem*>(index.internalPointer());
    ProjectTreeItem &parentItem = childItem->getParent();

    if ( &parentItem == mpRoot )
        return QModelIndex();

    return createIndex(parentItem.row(), 0, &parentItem);
}

int ProjectModel::rowCount(const QModelIndex &parent) const
{
    ProjectTreeItem *parentItem;
    if ( parent.column() > 0 )
        return 0;

    if ( !parent.isValid() )
        parentItem = mpRoot;
    else
        parentItem = static_cast<ProjectTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
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

    ProjectTreeItem *item = static_cast<ProjectTreeItem*>(index.internalPointer());
    return item->data();
}

QVariant ProjectModel::resourceData(const QModelIndex& index)
{
    if ( !index.isValid() )
        return QVariant();

    ProjectTreeItem *item = static_cast<ProjectTreeItem*>(index.internalPointer());
    return item->resourceData();
}

// - Slots

void ProjectModel::onDataChanged()
{
    //beginResetModel();
    //endResetModel();
}

// - Operations

void ProjectModel::buildTree()
{
    ProjectTreeItem* pscripts = new ProjectTreeTextItem("Scripts");
    mpRoot->addChild(pscripts);

    ProjectTreeItem* pworlds = new ProjectTreeTextItem("Worlds");
    mpRoot->addChild(pworlds);

    Project::Worlds& worlds = mpProject->getWorlds();
    for ( int index = 0; index < worlds.size(); ++index )
    {
        TileWorld& world = *worlds[index];
        ProjectTreeItem* pitem = new ProjectTreeWorldItem(world);
        pworlds->addChild(pitem);
    }
}
