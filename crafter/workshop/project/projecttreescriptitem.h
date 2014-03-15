#ifndef PROJECTTREESCRIPTITEM_H
#define PROJECTTREESCRIPTITEM_H

#include "projecttreeitem.h"

#include "../script/scripthandle.h"

class ProjectTreeScriptItem : public ProjectTreeItem
{
public:
    explicit ProjectTreeScriptItem(const ScriptHandle& script);

  // get/set
    ScriptHandle& getScript();

  // query
    virtual QVariant data() override;
    virtual QVariant resourceData() override;

private:

  // data
    ScriptHandle mScript;
};

#endif // PROJECTTREESCRIPTITEM_H
