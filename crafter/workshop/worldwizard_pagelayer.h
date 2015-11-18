#ifndef WORLDWIZARD_PAGELAYER_H
#define WORLDWIZARD_PAGELAYER_H

#include <QWizardPage>

namespace Ui {
class WorldWizard_PageLayer;
}

class WorldWizard_PageLayer : public QWizardPage
{
    Q_OBJECT

public:
    explicit WorldWizard_PageLayer(QWidget *parent = 0);
    ~WorldWizard_PageLayer();

public slots:
    void on_checkCreate_statechanged(int state);

private:
  // operations
    void fillTileSets();

  // data
    Ui::WorldWizard_PageLayer *ui;
};

#endif // WORLDWIZARD_PAGELAYER_H
