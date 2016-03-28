#include "newentitydialog.h"
#include "ui_newentitydialog.h"

#include "entity/entity.h"
#include "entity/physicscomponent.h"
#include "entity/spritecomponent.h"

void NewEntityDialog::edit(Entity& entity)
{
    NewEntityDialog dlg;
    dlg.setEntity(entity);
    if ( dlg.exec() == DialogCode::Accepted )
    {
    }
}

NewEntityDialog::NewEntityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEntityDialog)
{
    ui->setupUi(this);
}

NewEntityDialog::~NewEntityDialog()
{
    delete ui;
}

// - Operations

void NewEntityDialog::setEntity(Entity& entity)
{
    ui->editName->setText(entity.getName());

    auto psprite = entity.component<SpriteComponent>();
    if ( psprite != nullptr )
    {

    }
    else
    {
        ui->tabSprite->setVisible(false);
    }

    auto pphysics = entity.component<PhysicsComponent>();
    if ( pphysics != nullptr )
    {
        ui->chkDynamic->setChecked(!pphysics->isStatic());
        ui->chkRotate->setChecked(!pphysics->isFixedRotation());
    }
    else
    {
        ui->tabPhysics->setVisible(false);
    }
}
