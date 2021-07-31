#include "newentitydialog.h"
#include "ui_newentitydialog.h"

#include <memory>

#include <QDir>
#include <QPixmap>
#include <QListWidgetItem>
#include <QTimer>
#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>

#include "entity/entity.h"
#include "entity/physicscomponent.h"
#include "entity/spritecomponent.h"
#include "entity/spriteanimation.h"

#include "selectanimationdialog.h"
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
    mAnimImages(),
    mAnimationModel()
{
    ui->setupUi(this);


    connect(ui->listComponents, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), SLOT(on_componentSelected(QListWidgetItem*)));
    connect(ui->treeAnimations, SIGNAL(clicked(const QModelIndex&)), SLOT(on_animationTreeClicked(const QModelIndex&)));

    ui->listComponents->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listComponents, SIGNAL(customContextMenuRequested(QPoint)), SLOT(on_componentContextMenu(QPoint)));

    ui->treeAnimations->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeAnimations, SIGNAL(customContextMenuRequested(QPoint)), SLOT(on_animationContextMenu(QPoint)));

    ui->treeAnimations->setModel(&mAnimationModel);

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

    connect(ui->actionAddAnimation, SIGNAL(triggered()), SLOT(on_addAddAnimation()));
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

void NewEntityDialog::on_animationContextMenu(const QPoint& position)
{
    QMenu menu(this);

    auto index = ui->treeAnimations->indexAt(position);
    if (index.isValid()) {
        QVariant data = mAnimationModel.actualData(index);
        if (data.canConvert<SpriteAnimation>()) {
            // selected an animation
            menu.addAction(ui->actionAddTile);
        }
        else {
            // selected an animation tile
        }
    }
    else {
        // white space
        menu.addAction(ui->actionAddAnimation);
    }

    menu.exec(ui->treeAnimations->mapToGlobal(position));
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
            else {
                mpEntity->removeComponent<PhysicsComponent>();
            }
        }

        qDeleteAll(selection);

        if (ui->listComponents->count() == 0) {
            // no more components to navigate to
            ui->stackedWidget->setVisible(false);
        }
    }
}

void NewEntityDialog::on_addAddAnimation()
{
    SelectAnimationDialog dialog;
    QString animationType = dialog.exec();
    if (!animationType.isEmpty()){
        auto psprite = mpEntity->component<SpriteComponent>();
        SpriteAnimation animation;
        animation.setName(animationType);
        psprite->addAnimation(std::move(animation));
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
            ui->treeAnimations->setVisible(false);
        }
        else {
            mAnimationModel.setSprite(*psprite);
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

void NewEntityDialog::on_animationTreeClicked(const QModelIndex& index)
{
    /*
    if (current->parent() == nullptr) {
        mAnimImages.clear();
        mAnimIndex = 0;

        int row = ui->treeAnimations->indexOfTopLevelItem(current);

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
    */
}

void NewEntityDialog::on_animationTimeout()
{
    mAnimIndex++;
    if ( mAnimIndex >= mAnimImages.size() ) {
        mAnimIndex = 0;
    }
    ui->labelImage->setPixmap(mAnimImages[mAnimIndex]);
}
