#ifndef LAYERWIZARD_PAGETILESET_H
#define LAYERWIZARD_PAGETILESET_H

#include <QWizardPage>

namespace Ui {
class LayerWizard_PageTileSet;
}

class LayerWizard_PageTileSet : public QWizardPage
{
    Q_OBJECT

public:
    explicit LayerWizard_PageTileSet(QWidget *parent = 0);
    ~LayerWizard_PageTileSet();

public slots:
    void on_action_toggled(bool checked);

private:
    Ui::LayerWizard_PageTileSet *ui;
};

#endif // LAYERWIZARD_PAGETILESET_H
