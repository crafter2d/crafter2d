#ifndef NEWENTITYDIALOG_H
#define NEWENTITYDIALOG_H

#include <QDialog>

#include "models/spriteanimationmodel.h"

namespace Ui {
class NewEntityDialog;
}

class Entity;
class QListWidgetItem;
class QTreeWidgetItem;
class QTimer;
class SpriteAnimation;

class NewEntityDialog : public QDialog
{
    Q_OBJECT

public:
    static void edit(Entity& entity);

    explicit NewEntityDialog(QWidget *parent = 0);
    ~NewEntityDialog();

private slots:
    void on_componentSelected(QListWidgetItem* pselected);
    void on_componentContextMenu(const QPoint& position);
    void on_animationContextMenu(const QPoint& position);
    void on_animationSelected(const QModelIndex& index, const QModelIndex&);
    void on_animationTimeout();
    void on_addSpriteComponent();
    void on_addPhysicsComponent();
    void on_deleteComponent();
    void on_addAddAnimation();

private:

    void createContextMenu();
    void setEntity(Entity& entity);

    std::vector<QPixmap> mAnimImages;
    SpriteAnimationModel mAnimationModel;
    Ui::NewEntityDialog *ui;
    Entity* mpEntity; // not owning
    QTimer* mpAnimationTimer;    
    size_t mAnimIndex;
};

#endif // NEWENTITYDIALOG_H
