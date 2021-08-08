#include "physicscomponentwidget.h"
#include "ui_physicscomponentwidget.h"

#include "entity/physicscomponent.h"

PhysicsComponentWidget::PhysicsComponentWidget(PhysicsComponent &component, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhysicsComponentWidget),
    mComponent(component)
{
    ui->setupUi(this);

    ui->chkDynamic->setChecked(!component.isStatic());
    ui->chkRotate->setChecked(!component.isFixedRotation());
}

PhysicsComponentWidget::~PhysicsComponentWidget()
{
    delete ui;
}
