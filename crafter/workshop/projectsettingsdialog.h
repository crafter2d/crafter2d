#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include <QDialog>

#include "project/projectsettings.h"

namespace Ui {
class ProjectSettingsDialog;
}

class ProjectSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    ProjectSettingsDialog(QWidget* pparent = nullptr);

private slots:
    void on_addPath_triggered();
    void on_commitData(QWidget* peditor);
    void on_ok();

private:
  // members
    Ui::ProjectSettingsDialog* ui;
    ProjectSettings mSettings;
};

#endif // PROJECTSETTINGSDIALOG_H
