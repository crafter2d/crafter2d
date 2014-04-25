#ifndef LAYERWIZARD_PAGETILESET_H
#define LAYERWIZARD_PAGETILESET_H

#include <QWizardPage>

namespace Ui {
class LayerWizard_PageTileSet;
}

class QItemSelection;

class TileSetModel;

class LayerWizard_PageTileSet : public QWizardPage
{
    Q_OBJECT

public:
    explicit LayerWizard_PageTileSet(QWidget *parent = 0);
    ~LayerWizard_PageTileSet();

protected slots:
    void on_action_toggled(bool checked);
    void on_listSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    Ui::LayerWizard_PageTileSet *ui;

    TileSetModel* mpTileSetModel;
};

#endif // LAYERWIZARD_PAGETILESET_H
