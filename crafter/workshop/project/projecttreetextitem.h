#ifndef PROJECTTREETEXTITEM_H
#define PROJECTTREETEXTITEM_H

#include <QString>

#include "projecttreeitem.h"

class ProjectTreeTextItem : public ProjectTreeItem
{
public:
    ProjectTreeTextItem(const QString& text);

  // query
    virtual QVariant data() override;

private:

  // data
    QString mText;
};

#endif // PROJECTTREETEXTITEM_H
