#ifndef WORLDWIZARD_PAGEWORLD_H
#define WORLDWIZARD_PAGEWORLD_H

#include <QWizardPage>

namespace Ui {
class WorldWizard_PageWorld;
}

class WorldWizard_PageWorld : public QWizardPage
{
    Q_OBJECT

public:
    explicit WorldWizard_PageWorld(QWidget *parent = 0);
    ~WorldWizard_PageWorld();

  // query
    QString getName() const;

private:
    Ui::WorldWizard_PageWorld *ui;
};

#endif // WORLDWIZARD_PAGEWORLD_H
