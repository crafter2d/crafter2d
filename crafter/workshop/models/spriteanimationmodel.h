#ifndef SPRITEANIMATIONMODEL_H
#define SPRITEANIMATIONMODEL_H

#include <QAbstractItemModel>
#include <memory>

class SpriteComponent;

class TreeItem {
public:
    using DataFnc = std::function<QVariant(TreeItem&,int)>;
    enum class Kind {
        eRoot, eAnimation, eTile
    };

    TreeItem(Kind kind, void* pdata, DataFnc pdataFnc): mKind(kind), mpData(pdata), mDataFnc(std::move(pdataFnc)), mpParent(nullptr) {}

    bool isAnimation() const { return mKind == Kind::eAnimation; }

    TreeItem* getParent() { return mpParent; }
    void add(std::unique_ptr<TreeItem> item) { item->mpParent = this; mChildren.push_back(std::move(item)); }
    void clear() { mChildren.clear(); }
    size_t size() const { return mChildren.size(); }
    const TreeItem* getItem(int index) const { return index < mChildren.size() ? mChildren[index].get() : nullptr; }
    QVariant data(int col) { return mDataFnc(*this, col); }
    int row() const { return mpParent ? mpParent->indexOf(*this) : -1; }
    int indexOf(const TreeItem& item) const {
        auto it = std::find_if(mChildren.begin(), mChildren.end(), [&item](const std::unique_ptr<TreeItem>& that) { return &item == that.get(); });
        return it != mChildren.end();
    }

    template<class T>
    T& get() {
        return *(static_cast<T*>(mpData));
    }

private:
    Kind mKind;
    void* mpData;
    DataFnc mDataFnc;
    TreeItem* mpParent;
    std::vector<std::unique_ptr<TreeItem>> mChildren;
};

class SpriteAnimationModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    SpriteAnimationModel(QObject* parent = nullptr);

    void setSprite(SpriteComponent& sprite);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &/*parent*/) const override;
    QVariant headerData(int section, Qt::Orientation /*orientation*/, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    QVariant actualData(const QModelIndex& index) const;

private slots:
    void on_animationsChanged();

private:

    SpriteComponent* mpSprite;
    TreeItem mRoot;
};

#endif // SPRITEANIMATIONMODEL_H
