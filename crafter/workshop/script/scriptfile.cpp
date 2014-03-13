#include "scriptfile.h"

ScriptFile::ScriptFile():
    QObject(0),
    mPath()
{
}

ScriptFile::ScriptFile(const QString& path):
    QObject(0),
    mPath(path)
{
}

ScriptFile::ScriptFile(const ScriptFile& that):
    QObject(0),
    mPath(that.mPath)
{
}

// - Get/set

const QString& ScriptFile::getPath() const
{
    return mPath;
}

