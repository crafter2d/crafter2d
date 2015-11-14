#ifndef RESOURCEHANDLE_H
#define RESOURCEHANDLE_H

#include <QObject>

class Resource;

class ResourceHandle : public QObject
{
    Q_OBJECT
public:
             ResourceHandle();
             ResourceHandle(const ResourceHandle& handle);
    explicit ResourceHandle(Resource& resource);

    ResourceHandle& operator=(const ResourceHandle& that);
    ResourceHandle& operator=(Resource& that);

    const QString& getResourceName() const;


    Resource* operator->() { Q_ASSERT(mpResource != nullptr); return mpResource; }
    Resource& operator*()  { Q_ASSERT(mpResource != nullptr); return *mpResource; }

private:

    Resource* mpResource;
};

Q_DECLARE_METATYPE(ResourceHandle)

#endif // RESOURCEHANDLE_H
