#include "newentitydialog.h"
#include "ui_newentitydialog.h"

#include <QDir>
#include <QPixmap>
#include <QListWidgetItem>
#include <QTimer>

#include "entity/entity.h"
#include "entity/physicscomponent.h"
#include "entity/spritecomponent.h"
#include "entity/spriteanimation.h"

#include "project.h"

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
    ui(new Ui::NewEntityDialog),
    mpEntity(nullptr),
    mpAnimationTimer(nullptr),
    mAnimImages()
{
    ui->setupUi(this);

    connect(ui->listComponents, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), SLOT(on_componentSelected(QListWidgetItem*)));
    connect(ui->listAnimations, SIGNAL(currentRowChanged(int)), SLOT(on_animationSelected(int)));
}

NewEntityDialog::~NewEntityDialog()
{
    delete ui;
}

// - Operations

void NewEntityDialog::setEntity(Entity& entity)
{
    mpEntity = &entity;

    ui->editName->setText(entity.getName());

    auto psprite = entity.component<SpriteComponent>();
    if ( psprite != nullptr ) {
        if ( psprite->getAnimations().empty() ) {
            QPixmap pixmap(Project::getActiveProject().getImagePath() + QDir::separator() + psprite->getTexture());
            ui->labelImage->setPixmap(pixmap);
            ui->listAnimations->setVisible(false);
        }
        else {
            for ( auto& anim : psprite->getAnimations() )
            {
                ui->listAnimations->addItem(anim.getName());
            }
            ui->listAnimations->setCurrentRow(0);
        }

        ui->listComponents->addItem("Sprite");
    }

    auto pphysics = entity.component<PhysicsComponent>();
    if ( pphysics != nullptr ) {
        ui->chkDynamic->setChecked(!pphysics->isStatic());
        ui->chkRotate->setChecked(!pphysics->isFixedRotation());
        ui->listComponents->addItem("Physics");
    }
}

// - Slots
void NewEntityDialog::on_componentSelected(QListWidgetItem* pselected)
{
    if ( pselected ) {
        if ( pselected->text() == "Sprite" ) {
            ui->stackedWidget->setCurrentWidget(ui->pageSprite);
        }
        else {
            ui->stackedWidget->setCurrentWidget(ui->page_2);
        }
    }
}

void NewEntityDialog::on_animationSelected(int row)
{
    mAnimImages.clear();
    mAnimIndex = 0;

    auto* psprite = mpEntity->component<SpriteComponent>();
    auto& animation = psprite->getAnimations()[row];
    for ( auto& tile : animation.getTiles() ) {
        mAnimImages.emplace_back(Project::getActiveProject().getImagePath() + QDir::separator() + tile.getName());
    }

    if ( mpAnimationTimer == nullptr ) {
        mpAnimationTimer = new QTimer(this);
        connect(mpAnimationTimer, SIGNAL(timeout()), SLOT(on_animationTimeout()));
        mpAnimationTimer->setInterval(psprite->getAnimationSpeed());
        mpAnimationTimer->start();
    }
}

void NewEntityDialog::on_animationTimeout()
{
    mAnimIndex++;
    if ( mAnimIndex >= mAnimImages.size() ) {
        mAnimIndex = 0;
    }
    ui->labelImage->setPixmap(mAnimImages[mAnimIndex]);
}
