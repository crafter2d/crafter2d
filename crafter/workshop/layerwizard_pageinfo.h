#ifndef LAYERWIZARD_PAGEINFO_H
#define LAYERWIZARD_PAGEINFO_H

#include <QWizardPage>

namespace Ui {
class LayerWizard_PageInfo;
}

class LayerWizard_PageInfo : public QWizardPage
{
    Q_OBJECT

public:
    explicit LayerWizard_PageInfo(QWidget *parent = 0);
    ~LayerWizard_PageInfo();

public slots:
    void on_checkCreate_statechanged(int state);

private:
    Ui::LayerWizard_PageInfo *ui;
};

#endif // LAYERWIZARD_PAGEINFO_H
