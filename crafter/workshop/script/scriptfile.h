#ifndef SCRIPTFILE_H
#define SCRIPTFILE_H

#include <QObject>

class ScriptFile : public QObject
{
public:
    ScriptFile();
    ScriptFile(const QString &path);
    ScriptFile(const ScriptFile& that);

  // get/set
    const QString& getPath() const;

private:

  // data
    QString mPath;
};

Q_DECLARE_METATYPE(ScriptFile)

#endif // SCRIPTFILE_H
