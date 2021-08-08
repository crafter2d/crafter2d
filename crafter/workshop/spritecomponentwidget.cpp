#include "spritecomponentwidget.h"
#include "ui_spritecomponentwidget.h"

#include <QDir>
#include <QMenu>
#include <QTimer>

#include "project.h"
#include "selectanimationdialog.h"

SpriteComponentWidget::SpriteComponentWidget(SpriteComponent& component, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpriteComponentWidget),
    mComponent(component),
    mpAnimationTimer(nullptr),
    mAnimIndex(0)
{
    ui->setupUi(this);

    ui->treeAnimations->setModel(&mAnimationModel);
    ui->checkAnimate->setChecked(!component.getAnimations().empty());

    auto pselectionModel = ui->treeAnimations->selectionModel();
    connect(pselectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(on_animationSelected(QModelIndex,QModelIndex)));
    connect(ui->checkAnimate, SIGNAL(stateChanged(int)), SLOT(on_animateStateChanged(int)));

    if ( component.getAnimations().empty() ) {
        QPixmap pixmap(Project::getActiveProject().getImagePath() + QDir::separator() + component.getTexture());
        ui->labelImage->setPixmap(pixmap);
    }
    else {
        mAnimationModel.setSprite(component);
        ui->treeAnimations->setCurrentIndex(mAnimationModel.index(0, 0));
    }

    ui->treeAnimations->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeAnimations, SIGNAL(customContextMenuRequested(QPoint)), SLOT(on_animationContextMenu(QPoint)));

    createContextMenu();
}

SpriteComponentWidget::~SpriteComponentWidget()
{
    if (mpAnimationTimer) {
        mpAnimationTimer->stop();
        delete mpAnimationTimer;
    }
    delete ui;
}

void SpriteComponentWidget::createContextMenu() {
    connect(ui->actionAddAnimation, SIGNAL(triggered()), SLOT(on_addAddAnimation()));
}

void SpriteComponentWidget::on_animationContextMenu(const QPoint &position)
{
    QMenu menu(this);

    auto index = ui->treeAnimations->indexAt(position);
    if (index.isValid()) {
        QVariant data = mAnimationModel.actualData(index);
        if (data.canConvert<SpriteAnimation>()) {
            // selected an animation
            menu.addAction(ui->actionAdd);
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

void SpriteComponentWidget::on_animationSelected(const QModelIndex &index, const QModelIndex &)
{
    if (index.isValid()) {
        QVariant data = mAnimationModel.actualData(index);
        if (data.canConvert<SpriteAnimation>()) {
            auto panimation = static_cast<SpriteAnimation*>(data.data());

            mAnimImages.clear();
            mAnimIndex = 0;

            for ( auto& tile : panimation->getTiles() ) {
                mAnimImages.emplace_back(Project::getActiveProject().getImagePath() + QDir::separator() + tile.getName());
            }

            if ( mpAnimationTimer == nullptr ) {
                auto speed = mComponent.getAnimationSpeed();
                mpAnimationTimer = new QTimer(this);
                connect(mpAnimationTimer, SIGNAL(timeout()), SLOT(on_animationTimeout()));
                mpAnimationTimer->setInterval(speed);
                mpAnimationTimer->start();
            }
        }
    }
}

void SpriteComponentWidget::on_animationTimeout()
{
    mAnimIndex++;
    if ( mAnimIndex >= mAnimImages.size() ) {
        mAnimIndex = 0;
    }
    ui->labelImage->setPixmap(mAnimImages[mAnimIndex]);
}

void SpriteComponentWidget::on_animateStateChanged(int)
{
    if (ui->checkAnimate->isChecked()) {
        ui->treeAnimations->setVisible(true);
    }
    else {
        ui->treeAnimations->setVisible(false);

        mComponent.clearAnimations();
    }
}

void SpriteComponentWidget::on_addAddAnimation()
{
    SelectAnimationDialog dialog;
    QString animationType = dialog.exec();
    if (!animationType.isEmpty()){
        SpriteAnimation animation;
        animation.setName(animationType);
        mComponent.addAnimation(std::move(animation));
    }
}
