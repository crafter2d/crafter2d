#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"

#include <QFileDialog>

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

// - Get/set

const QString NewProjectDialog::getName() const
{
   return ui->edtName->displayText();
}

const QString NewProjectDialog::getPath() const
{
    return ui->editFolder->displayText();
}

// - Slots

void NewProjectDialog::on_buttonBrowse_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Project Folder");
    if ( !path.isEmpty() )
    {
        ui->editFolder->setText(path);
    }
}
