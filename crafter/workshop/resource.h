#ifndef RESOURCE_H
#define RESOURCE_H

#include <QObject>
#include <QString>

class Resource : public QObject
{
    Q_OBJECT

public:
    enum Type { eWorld, eTileSet, eScript, eObject };

  // get/set
    Type getType() const { return mType; }

    const QString& getResourceName() const;
    void           setResourceName(const QString& name);

    virtual bool isDirty() const;
            void setDirty(bool dirty);

protected:
    explicit Resource(Type type);
             Resource(const Resource& that);
    Resource& operator=(const Resource& that);

private:

  // data
    Type    mType;
    QString mResourceName;
    bool    mDirty;
};

#endif // RESOURCE_H
