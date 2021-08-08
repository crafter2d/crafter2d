#ifndef PHYSICSCOMPONENTWIDGET_H
#define PHYSICSCOMPONENTWIDGET_H

#include <QWidget>

class PhysicsComponent;

namespace Ui {
class PhysicsComponentWidget;
}

class PhysicsComponentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhysicsComponentWidget(PhysicsComponent& component, QWidget *parent = nullptr);
    ~PhysicsComponentWidget();

private:
    Ui::PhysicsComponentWidget *ui;
    PhysicsComponent& mComponent;
};

#endif // PHYSICSCOMPONENTWIDGET_H
