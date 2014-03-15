#include "projecttreescriptitem.h"

#include <QFileInfo>

#include "../script/scriptfile.h"

ProjectTreeScriptItem::ProjectTreeScriptItem(const ScriptHandle& script):
    ProjectTreeItem(ProjectTreeItem::eScript),
    mScript(script)
{
}

ScriptHandle& ProjectTreeScriptItem::getScript()
{
    return mScript;
}

QVariant ProjectTreeScriptItem::data()
{
    return QFileInfo(mScript->getPath()).baseName();
}

QVariant ProjectTreeScriptItem::resourceData()
{
    return QVariant::fromValue(mScript);
}
