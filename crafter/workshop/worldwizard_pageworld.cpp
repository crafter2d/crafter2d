#include "worldwizard_pageworld.h"
#include "ui_worldwizard_pageworld.h"

#include "worldwizard.h"

WorldWizard_PageWorld::WorldWizard_PageWorld(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WorldWizard_PageWorld)
{
    ui->setupUi(this);

    registerField("world.name", ui->editName);
    registerField("world.isTopDown", ui->radioTopDown);
}

WorldWizard_PageWorld::~WorldWizard_PageWorld()
{
    delete ui;
}

// - Query

QString WorldWizard_PageWorld::getName() const
{
    return ui->editName->text();
}
