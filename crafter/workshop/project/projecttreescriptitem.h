#ifndef PROJECTTREESCRIPTITEM_H
#define PROJECTTREESCRIPTITEM_H

#include "projecttreeitem.h"

class ScriptFile;

class ProjectTreeScriptItem : public ProjectTreeItem
{
public:
    explicit ProjectTreeScriptItem(ScriptFile &script);

  // get/set
    ScriptFile& getScript();

  // query
    virtual QVariant data() override;
    virtual QVariant resourceData() override;

private:

  // data
    ScriptFile& mScript;
};

#endif // PROJECTTREESCRIPTITEM_H
