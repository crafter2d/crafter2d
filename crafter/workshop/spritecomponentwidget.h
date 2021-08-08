#ifndef SPRITECOMPONENTWIDGET_H
#define SPRITECOMPONENTWIDGET_H

#include <QWidget>

#include "entity/spritecomponent.h"
#include "models/spriteanimationmodel.h"

namespace Ui {
class SpriteComponentWidget;
}

class SpriteComponentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpriteComponentWidget(SpriteComponent& component, QWidget *parent = nullptr);
    ~SpriteComponentWidget();

private slots:
    void on_animationContextMenu(const QPoint& position);
    void on_animationSelected(const QModelIndex& index, const QModelIndex&);
    void on_animationTimeout();
    void on_animateStateChanged(int);
    void on_addAddAnimation();


private:
    void createContextMenu();

    Ui::SpriteComponentWidget *ui;
    SpriteComponent& mComponent;
    SpriteAnimationModel mAnimationModel;
    std::vector<QPixmap> mAnimImages;
    QTimer* mpAnimationTimer;
    size_t mAnimIndex;
};

#endif // SPRITECOMPONENTWIDGET_H
