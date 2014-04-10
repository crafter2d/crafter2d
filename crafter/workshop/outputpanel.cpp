#include "outputpanel.h"
#include "ui_outputpanel.h"

#include "project/projectmanager.h"
#include "project.h"

OutputPanel::OutputPanel(MainWindow& window) :
    DockPanel(window),
    ui(new Ui::OutputPanel)
{
    ui->setupUi(this);

    ProjectManager& manager = ProjectManager::getInstance();
    connect(&manager, SIGNAL(projectChanged(Project*)), this, SLOT(on_projectChanged(Project*)));

    Project* pactiveproject = manager.getActiveProject();
    if ( pactiveproject != nullptr )
    {
        on_projectChanged(pactiveproject);
    }
}

OutputPanel::~OutputPanel()
{
    delete ui;
}

// - Notifications

void OutputPanel::worldActivated(TileWorld* /* pworld */)
{
    // do nothing, we should change this
}

// - Slots

void OutputPanel::on_projectChanged(Project* pproject)
{
    if ( pproject != nullptr )
    {
        connect(pproject, SIGNAL(messageAvailable(QString)), this, SLOT(on_messageAvailable(QString)));
    }
}

void OutputPanel::on_messageAvailable(const QString& msg)
{
    ui->listWidget->addItem(msg);
    ui->listWidget->scrollToBottom();
}
