#include "scripthandle.h"

ScriptHandle::ScriptHandle(QObject* parent):
    QObject(parent)
{
}

ScriptHandle::ScriptHandle(const ScriptHandle &that):
    QObject(0),
    mpScript(that.mpScript)
{
}

ScriptHandle::ScriptHandle(ScriptFile &script):
    QObject(0),
    mpScript(&script)
{
}

void ScriptHandle::operator =(const ScriptHandle& that)
{
    mpScript = that.mpScript;
}

bool ScriptHandle::operator==(const ScriptFile& that) const
{
    return mpScript == &that;
}

// - Operations

ScriptFile& ScriptHandle::operator *()
{
    return *mpScript;
}

ScriptFile* ScriptHandle::operator ->()
{
    return mpScript;
}
