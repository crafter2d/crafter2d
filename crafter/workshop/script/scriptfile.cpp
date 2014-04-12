#include "scriptfile.h"

#include <QFile>
#include <QPlainTextDocumentLayout>

ScriptFile::ScriptFile():
    Resource(),
    mPath(),
    mDocument(),
    mLoaded(false)
{
}

ScriptFile::ScriptFile(const QString& path):
    Resource(),
    mPath(path),
    mDocument(),
    mLoaded(false)
{
}

ScriptFile::ScriptFile(const ScriptFile& that):
    Resource(),
    mPath(that.mPath),
    mDocument(),
    mLoaded(false)
{
}

ScriptFile::~ScriptFile()
{
}

// - Get/set

const QString& ScriptFile::getPath() const
{
    return mPath;
}

QTextDocument& ScriptFile::getDocument()
{
    return mDocument;
}

// - Query

bool ScriptFile::isDirty() const
{
    return mDocument.isModified();
}

// - Operations

void ScriptFile::ensureLoaded()
{
    if ( !mLoaded )
    {
        QFile file(mPath);

        if ( file.open(QIODevice::ReadOnly) )
        {
            QString data(file.readAll());
            mDocument.setDocumentLayout(new QPlainTextDocumentLayout(&mDocument));
            mDocument.setPlainText(data);
        }
    }
}

void ScriptFile::save()
{
    QFile file(mPath);
    if ( file.open(QIODevice::WriteOnly) )
    {
        QByteArray array;
        QString text = mDocument.toPlainText();
        array.append(text);
        file.write(array);
    }
}
