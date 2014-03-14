#include "projecttreescriptitem.h"

#include <QFileInfo>

#include "../script/scriptfile.h"

ProjectTreeScriptItem::ProjectTreeScriptItem(ScriptFile &script):
    ProjectTreeItem(ProjectTreeItem::eScript),
    mScript(script)
{
}

ScriptFile& ProjectTreeScriptItem::getScript()
{
    return mScript;
}

QVariant ProjectTreeScriptItem::data()
{
    return QFileInfo(mScript.getPath()).baseName();
}

QVariant ProjectTreeScriptItem::resourceData()
{
    return QVariant::fromValue(mScript);
}
