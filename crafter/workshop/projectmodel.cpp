#include "projectmodel.h"

#include "project/projecttreetextitem.h"
#include "project/projecttreeobjectitem.h"
#include "project/projecttreerootitem.h"

#include "entity/entity.h"
#include "world/tileset.h"
#include "world/tileworld.h"
#include "script/scriptfile.h"

#include "project.h"

ProjectModel::ProjectModel():
    QAbstractItemModel(),
    mpProject(nullptr),
    mpRoot(new ProjectTreeRootItem())
{
    mpRoot->setRoot();
}

ProjectModel::~ProjectModel()
{
    delete mpRoot;
    mpRoot = nullptr;
}

// - Get/set

void ProjectModel::setProject(Project* pproject)
{
    if ( mpProject != pproject )
    {
        beginResetModel();

        if ( mpProject != nullptr )
        {
            disconnect(mpProject, SIGNAL(dataChanged()));
        }

        mpProject = pproject;

        if ( mpProject != nullptr )
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
    if ( childItem != nullptr )
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
    if ( parent.column() > 0 )
        return 0;

    ProjectTreeItem *parentItem = parent.isValid() ? static_cast<ProjectTreeItem*>(parent.internalPointer()) : mpRoot;
    return parentItem->childCount();
}

int ProjectModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QVariant ProjectModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const
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

    ProjectTreeItem *pitem = static_cast<ProjectTreeItem*>(index.internalPointer());
    return pitem->resourceData();
}

// - Slots

void ProjectModel::onDataChanged()
{
    beginResetModel();

    mpRoot->clear();
    buildTree();

    endResetModel();
}

// - Operations

void ProjectModel::buildTree()
{
    auto pentities = new ProjectTreeTextItem("Objects");
    mpRoot->addChild(pentities);

    auto& entities = mpProject->getEntities();
    for ( auto pentity : entities )
    {
        ResourceHandle handle(*pentity);
        auto pitem = new ProjectTreeObjectItem(handle);
        pentities->addChild(pitem);
    }

    auto pscripts = new ProjectTreeTextItem("Scripts");
    mpRoot->addChild(pscripts);

    Project::Scripts& scripts = mpProject->getScripts();
    for ( auto pscript : scripts )
    {
        ResourceHandle handle(*pscript);
        auto pitem = new ProjectTreeObjectItem(handle);
        pscripts->addChild(pitem);
    }

    auto ptilesets = new ProjectTreeTextItem("TileSets");
    mpRoot->addChild(ptilesets);

    auto& tilesets = mpProject->getTileSets();
    for ( auto ptileset : tilesets )
    {
        ResourceHandle handle(*ptileset);
        auto pitem = new ProjectTreeObjectItem(handle);
        ptilesets->addChild(pitem);
    }

    ProjectTreeItem* pworlds = new ProjectTreeTextItem("Worlds");
    mpRoot->addChild(pworlds);

    Project::Worlds& worlds = mpProject->getWorlds();
    for ( auto pworld : worlds )
    {
        ResourceHandle handle(*pworld);
        auto pitem = new ProjectTreeObjectItem(handle);
        pworlds->addChild(pitem);
    }
}
