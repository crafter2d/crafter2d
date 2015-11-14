#include "projecttreetextitem.h"

ProjectTreeTextItem::ProjectTreeTextItem(const QString &text):
    ProjectTreeItem(ProjectTreeItem::eText),
    mText(text)
{
}

QVariant ProjectTreeTextItem::data()
{
    return mText;
}
