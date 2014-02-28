#ifndef RESOURCE_H
#define RESOURCE_H

#include <QObject>
#include <QString>

class Resource : public QObject
{
    Q_OBJECT

public:
    Resource();

  // get/set
    const QString& getResourceName() const;
    void           setResourceName(const QString& name);

    bool isDirty() const;
    void setDirty(bool dirty);

private:

  // data
    QString mName;
    bool    mDirty;
};

#endif // RESOURCE_H
