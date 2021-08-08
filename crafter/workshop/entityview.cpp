#include "entityview.h"
#include "ui_entityview.h"

#include <QInputDialog>

#include "spritecomponentwidget.h"
#include "physicscomponentwidget.h"
#include "entity/entity.h"
#include "entity/spritecomponent.h"
#include "entity/physicscomponent.h"

EntityView::EntityView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntityView),
    mpAccordion(nullptr),
    mpEntity(nullptr)
{
    ui->setupUi(this);

    mpAccordion = new Accordion();
    ui->area->layout()->addWidget(mpAccordion);

    connect(ui->btnAddComponent, SIGNAL(triggered()), SLOT(on_addComponent()));

    /*
    QPalette whitepalette(palette());
    whitepalette.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(whitepalette);
    */
}

EntityView::~EntityView()
{
    delete ui;
}

void EntityView::setEntity(Entity &entity)
{
    mpEntity = &entity;
    if (mpEntity) {
        setWindowTitle(entity.getName());

        auto pspriteComponent = entity.component<SpriteComponent>();
        if (pspriteComponent)
            mpAccordion->addWidget("Sprite", new SpriteComponentWidget(*pspriteComponent));

        auto pphysicsComponent = entity.component<PhysicsComponent>();
        if (pphysicsComponent)
            mpAccordion->addWidget("Physics", new PhysicsComponentWidget(*pphysicsComponent));
    }
}

void EntityView::on_addComponent()
{
    QStringList items;
    items << "Sprite" << "Physics" << "Script";
    QInputDialog dialog;
    dialog.setComboBoxItems(items);
    if (dialog.exec() == QDialog::Accepted) {
        auto componentType = dialog.textValue();
        if (componentType == "Sprite") {
            auto pcomponent = new SpriteComponent();
            mpAccordion->addWidget("Sprite", new SpriteComponentWidget(*pcomponent));
            mpEntity->addComponent(pcomponent);
        }
        else if (componentType == "Physics") {
            auto pcomponent = new PhysicsComponent();
            mpAccordion->addWidget("Physics", new PhysicsComponentWidget(*pcomponent));
            mpEntity->addComponent(pcomponent);
        }
    }
}
