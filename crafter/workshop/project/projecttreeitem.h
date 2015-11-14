#ifndef PROJECTTREEITEM_H
#define PROJECTTREEITEM_H

#include <QList>
#include <QVariant>

class ProjectTreeItem
{
public:

    enum Kind { eRoot, eText, eObject, eInvalid };

    virtual ~ProjectTreeItem();

  // get/set
    Kind getKind() const;

    bool             hasParent() const;
    ProjectTreeItem& getParent();
    void             setParent(ProjectTreeItem* pparent);

    bool isRoot() const;
    void setRoot();

  // query
    int row() const;
    int childCount() const;
    ProjectTreeItem* child(int index);
    int indexOf(const ProjectTreeItem &item) const;

    virtual QVariant data();
    virtual QVariant resourceData();

  // operations
    void addChild(ProjectTreeItem* pitem);
    void clear();

protected:
    explicit ProjectTreeItem(Kind kind);

private:

  // data
    Kind mKind;
    ProjectTreeItem* mpParent;
    QList<ProjectTreeItem*> mChildren;
    QVariant mData;
};

#endif // PROJECTTREEITEM_H
