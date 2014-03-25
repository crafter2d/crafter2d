#ifndef PROJECTTREETEXTITEM_H
#define PROJECTTREETEXTITEM_H

#include <QString>

#include "projecttreeitem.h"

class ProjectTreeTextItem : public ProjectTreeItem
{
public:
    enum Category
    {
        eWorld,
        eScript
    };

    ProjectTreeTextItem(Category category);

  // get/set
    Category getCategory();

  // query
    virtual QVariant data() override;

private:

  // data
    Category mCategory;
};

#endif // PROJECTTREETEXTITEM_H
