#include "projecttreetextitem.h"

ProjectTreeTextItem::ProjectTreeTextItem(Category category):
    ProjectTreeItem(ProjectTreeItem::eText),
    mCategory(category)
{
}

ProjectTreeTextItem::Category ProjectTreeTextItem::getCategory()
{
    return mCategory;
}

QVariant ProjectTreeTextItem::data()
{
    switch ( mCategory )
    {
    case eWorld:
        return "Worlds";
    case eScript:
        return "Scripts";
    }

    return QVariant("<invalid>");
}
