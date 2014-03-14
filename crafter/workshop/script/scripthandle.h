#ifndef SCRIPTHANDLE_H
#define SCRIPTHANDLE_H

#include <QObject>

class ScriptFile;

class ScriptHandle : public QObject
{
    Q_OBJECT

public:
    ScriptHandle(QObject* parent = 0);
    ScriptHandle(const ScriptHandle& that);
    ScriptHandle(ScriptFile& script);

    void operator=(const ScriptHandle& that);

  // operations
    ScriptFile& operator*();
    ScriptFile* operator->();

private:

  // data
    ScriptFile* mpScript;
};

Q_DECLARE_METATYPE(ScriptHandle)

#endif // SCRIPTHANDLE_H
