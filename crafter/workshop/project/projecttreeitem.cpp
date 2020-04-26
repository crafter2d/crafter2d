#include "projecttreeitem.h"

#include <QtGlobal>

ProjectTreeItem::ProjectTreeItem(Kind kind):
    mKind(kind),
    mpParent(NULL),
    mChildren(),
    mData()
{
}

ProjectTreeItem::~ProjectTreeItem()
{
    clear();
}

// - Get/set

ProjectTreeItem::Kind ProjectTreeItem::getKind() const
{
    return mKind;
}

bool ProjectTreeItem::hasParent() const
{
    return mpParent != NULL;
}

ProjectTreeItem& ProjectTreeItem::getParent()
{
    Q_ASSERT(mpParent != NULL);
    return *mpParent;
}

void ProjectTreeItem::setParent(ProjectTreeItem* pparent)
{
    mpParent = pparent;
}

bool ProjectTreeItem::isRoot() const
{
    return mKind == eRoot;
}

void ProjectTreeItem::setRoot()
{
    mKind = eRoot;
}

// - Query

int ProjectTreeItem::row() const
{
    if ( mpParent != nullptr )
    {
        return mpParent->indexOf(*this);
    }
    return 0;
}

int ProjectTreeItem::childCount() const
{
    return mChildren.size();
}

ProjectTreeItem* ProjectTreeItem::child(int index)
{
    return mChildren.value(index);
}

void ProjectTreeItem::clear()
{
    qDeleteAll(mChildren);
    mChildren.clear();
}

int ProjectTreeItem::indexOf(const ProjectTreeItem& item) const
{
    return mChildren.indexOf(const_cast<ProjectTreeItem*>(&item));
}

QVariant ProjectTreeItem::data()
{
    return QVariant();
}

QVariant ProjectTreeItem::resourceData()
{
    return QVariant();
}

// - Operations

void ProjectTreeItem::addChild(ProjectTreeItem* pitem)
{
    pitem->setParent(this);
    mChildren.append(pitem);
}
