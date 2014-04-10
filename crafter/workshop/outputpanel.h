#ifndef OUTPUTPANEL_H
#define OUTPUTPANEL_H

#include "dockpanel.h"

namespace Ui {
class OutputPanel;
}

class Project;

class OutputPanel : public DockPanel
{
    Q_OBJECT

public:
    explicit OutputPanel(MainWindow &window);
    ~OutputPanel();

  // notifications
    virtual void worldActivated(TileWorld* pworld) override;

public slots:
    void on_projectChanged(Project* pproject);
    void on_messageAvailable(const QString& msg);

private:
    Ui::OutputPanel *ui;
};

#endif // OUTPUTPANEL_H
