#ifndef SCRIPTFILE_H
#define SCRIPTFILE_H

#include <QTextDocument>

#include "..\resource.h"

class ScriptFile : public Resource
{
public:
    ScriptFile();
    ScriptFile(const QString &path);
    ScriptFile(const ScriptFile& that);
    virtual ~ScriptFile();

  // get/set
    const QString& getPath() const;
    QTextDocument& getDocument();

  // query
    virtual bool isDirty() const override;

  // operations
    void ensureLoaded();
    void save();

private:

  // data
    QString mPath;
    QTextDocument mDocument;
    bool mLoaded;
};

#endif // SCRIPTFILE_H
