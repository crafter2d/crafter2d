#include "outputpanel.h"
#include "ui_outputpanel.h"

#include "project/projectmanager.h"
#include "project.h"
#include "mainwindow.h"

OutputPanel::OutputPanel(MainWindow& window) :
    DockPanel(window),
    ui(new Ui::OutputPanel)
{
    ui->setupUi(this);

    ProjectManager& manager = ProjectManager::getInstance();
    connect(&manager, SIGNAL(projectChanged(Project*)), this, SLOT(on_projectChanged(Project*)));

    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(on_item_doubleClicked(QListWidgetItem*)));

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

void OutputPanel::on_item_doubleClicked(QListWidgetItem *pitem)
{
    QString msg = pitem->text();

    int index = msg.indexOf('(');
    if ( index >= 0 )
    {
        QString classname = msg.left(index);

        int closeindex = msg.indexOf(')');
        int line = msg.mid(index + 1, closeindex - index - 1).toInt();

        getMainWindow().gotoError(classname, line);
    }
}
