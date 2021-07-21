#ifndef NEWENTITYDIALOG_H
#define NEWENTITYDIALOG_H

#include <QDialog>

namespace Ui {
class NewEntityDialog;
}

class Entity;
class QListWidgetItem;
class QTimer;

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
    void on_animationSelected(int row);
    void on_animationTimeout();
    void on_addSpriteComponent();
    void on_addPhysicsComponent();
    void on_deleteComponent();

private:

    void createContextMenu();
    void setEntity(Entity& entity);

    Ui::NewEntityDialog *ui;
    Entity* mpEntity;
    QTimer* mpAnimationTimer;
    std::vector<QPixmap> mAnimImages;
    size_t mAnimIndex;
};

#endif // NEWENTITYDIALOG_H
