#ifndef NEWSCRIPTDIALOG_H
#define NEWSCRIPTDIALOG_H

#include <QDialog>

class Project;
class ScriptFile;

namespace Ui {
class NewScriptDialog;
}

class NewScriptDialog : public QDialog
{
    Q_OBJECT

public:
    static ScriptFile *New(QWidget* parent, Project& project);

    explicit NewScriptDialog(QWidget *parent = 0);
    ~NewScriptDialog();

  // get/set
    QString getName() const;
    QString getBaseClass() const;

private:
    Ui::NewScriptDialog *ui;
};

#endif // NEWSCRIPTDIALOG_H
