#include "spriteanimationmodel.h"

#include "../entity/spritecomponent.h"
#include "../entity/spriteanimation.h"

SpriteAnimationModel::SpriteAnimationModel(QObject* parent):
    QAbstractItemModel(parent),
    mpSprite(nullptr),
    mRoot(TreeItem::Kind::eRoot, nullptr, nullptr)
{
}

QModelIndex SpriteAnimationModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    const TreeItem* pitem = nullptr;
    if (parent.isValid()) {
        pitem = static_cast<TreeItem*>(parent.internalPointer())->getItem(row);
    }
    else {
        pitem = mRoot.getItem(row);
    }

    if (pitem)
        return createIndex(row, column, reinterpret_cast<quintptr>(pitem));

    return QModelIndex();
}

int SpriteAnimationModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.column() > 0 )
        return 0;

    const TreeItem &parentItem = parent.isValid() ? *static_cast<TreeItem*>(parent.internalPointer()) : mRoot;
    return parentItem.size();
}

int SpriteAnimationModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QVariant SpriteAnimationModel::headerData(int section, Qt::Orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();

    switch (section)
    {
        case 0: return tr("Name");
    }

    return QVariant();
}

QVariant SpriteAnimationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

QModelIndex SpriteAnimationModel::parent(const QModelIndex &child) const
{
    if ( !child.isValid() )
        return QModelIndex();

    TreeItem *pchildItem = static_cast<TreeItem*>(child.internalPointer());
    TreeItem *pparentItem = pchildItem->getParent();

    if ( pparentItem == nullptr || pparentItem == &mRoot )
        return QModelIndex();

    return createIndex(pparentItem->indexOf(*pchildItem), 0, pparentItem);
}

QVariant SpriteAnimationModel::actualData(const QModelIndex &index) const
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->isAnimation() ? QVariant::fromValue(item->get<SpriteAnimation>()) : QVariant::fromValue(item->get<SpriteAnimationTile>());
}

void SpriteAnimationModel::on_animationsChanged()
{
    auto animationFnc = [](TreeItem& item, int col) -> QVariant {
        return QVariant(item.get<SpriteAnimation>().getName());
    };

    auto tileFnc = [](TreeItem& item, int col) -> QVariant {
        return QVariant(item.get<SpriteAnimationTile>().getName());
    };

    beginResetModel();

    mRoot.clear();
    if (mpSprite) {
        auto& animations = mpSprite->getAnimations();

        for (auto& anim : animations) {
            auto item = std::make_unique<TreeItem>(TreeItem::Kind::eAnimation, &anim, animationFnc);
            for (auto& tile : anim.getTiles()) {
                item->add(std::make_unique<TreeItem>(TreeItem::Kind::eTile, &tile, tileFnc));
            }
            mRoot.add(std::move(item));
        }
    }

    endResetModel();
}

void SpriteAnimationModel::setSprite(SpriteComponent& sprite)
{
    if (&sprite != mpSprite) {
        if (mpSprite) {
            disconnect(mpSprite, SIGNAL(dataChanged()));
        }

        mpSprite = &sprite;

        if (mpSprite) {
            connect(mpSprite, SIGNAL(dataChanged()), SLOT(on_animationsChanged()));
            on_animationsChanged();
        }
    }
}
