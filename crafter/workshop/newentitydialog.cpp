#include "newentitydialog.h"
#include "ui_newentitydialog.h"

#include <memory>

#include <QDir>
#include <QPixmap>
#include <QListWidgetItem>
#include <QTimer>
#include <QMenu>
#include <QContextMenuEvent>

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

    connect(ui->listComponents, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), SLOT(on_componentSelected(QListWidgetItem*)));
    connect(ui->listAnimations, SIGNAL(currentRowChanged(int)), SLOT(on_animationSelected(int)));

    ui->listComponents->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listComponents, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(on_componentContextMenu(const QPoint&)));

    createContextMenu();
}

NewEntityDialog::~NewEntityDialog()
{
    delete ui;
}

// - Operations

void NewEntityDialog::createContextMenu()
{
    connect(ui->actionSprite, SIGNAL(triggered()), SLOT(on_addSpriteComponent()));
    connect(ui->actionPhysics, SIGNAL(triggered()), SLOT(on_addPhysicsComponent()));
    connect(ui->actionDelete, SIGNAL(triggered()), SLOT(on_deleteComponent()));
}

void NewEntityDialog::on_componentContextMenu(const QPoint& position)
{
    QMenu menu(this);

    auto pselectedItem = ui->listComponents->itemAt(position);

    auto pcomponentMenu = new QMenu("Add", &menu);
    if (mpEntity) {
        ui->actionSprite->setDisabled(mpEntity->hasComponent<SpriteComponent>());
        pcomponentMenu->addAction(ui->actionSprite);
        ui->actionPhysics->setDisabled(mpEntity->hasComponent<PhysicsComponent>());
        pcomponentMenu->addAction(ui->actionPhysics);
    }

    menu.addMenu(pcomponentMenu);

    ui->actionDelete->setDisabled(pselectedItem == nullptr);
    menu.addAction(ui->actionDelete);

    menu.exec(ui->listComponents->mapToGlobal(position));
}

void NewEntityDialog::on_addSpriteComponent()
{
    mpEntity->addComponent(new SpriteComponent());

    auto item = new QListWidgetItem("Sprite");
    ui->listComponents->addItem(item);
    ui->listComponents->setItemSelected(item, true);

    ui->stackedWidget->setVisible(true);
}

void NewEntityDialog::on_addPhysicsComponent()
{
    mpEntity->addComponent(new PhysicsComponent());

    auto item = new QListWidgetItem("Physics");
    ui->listComponents->addItem(item);
    ui->listComponents->setCurrentItem(item);

    ui->stackedWidget->setVisible(true);
}

void NewEntityDialog::on_deleteComponent()
{
    auto selection = ui->listComponents->selectedItems();
    if (!selection.empty()) {
        foreach (QListWidgetItem* pitem, selection) {
            const auto& componentType = pitem->text();
            if (componentType == "Sprite") {
                mpEntity->removeComponent<SpriteComponent>();

                delete mpAnimationTimer;
                mpAnimationTimer = nullptr;
            }
            else
                mpEntity->removeComponent<PhysicsComponent>();
        }

        qDeleteAll(selection);

        if (ui->listComponents->count() == 0) {
            // no more components to navigate to
            ui->stackedWidget->setVisible(false);
        }
    }
}

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
            for ( auto& anim : psprite->getAnimations() ) {
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
            ui->stackedWidget->setCurrentWidget(ui->pagePhysics);
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
