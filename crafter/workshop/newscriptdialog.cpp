#include "newscriptdialog.h"
#include "ui_newscriptdialog.h"

#include "script/scriptfile.h"
#include "script/scriptbuilder.h"

#include "project.h"


// - Statics

ScriptFile* NewScriptDialog::New(QWidget* parent, Project& project)
{
    ScriptFile* pscript = nullptr;

    NewScriptDialog dialog(parent);
    if ( dialog.exec() == QDialog::Accepted )
    {
        QString name = dialog.getName();
        QString base = dialog.getBaseClass();

        pscript = ScriptBuilder::build(project, name, base);
        if ( pscript != nullptr )
        {
            project.addScript(pscript);
        }
    }
    return 0;
}

// - Implementation

NewScriptDialog::NewScriptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewScriptDialog)
{
    ui->setupUi(this);
}

NewScriptDialog::~NewScriptDialog()
{
    delete ui;
}

// - Get/set

QString NewScriptDialog::getName() const
{
    return ui->editName->displayText();
}

QString NewScriptDialog::getBaseClass() const
{
    return ui->selBaseClass->currentText();
}
