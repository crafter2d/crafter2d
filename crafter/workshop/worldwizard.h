#ifndef NEWWORLDWIZARD_H
#define NEWWORLDWIZARD_H

#include <QWizard>

class TileWorld;
class QTileSet;

class WorldWizard : public QWizard
{
    Q_OBJECT
public:

    enum {
        Page_World,
        Page_Layer,
        Page_TileSet,
        Page_Finalize
    };

    static TileWorld* show();

    explicit WorldWizard(QWidget *parent = 0);

    virtual int nextId() const override;

signals:

public slots:

private:

  // operations
    TileWorld*  createWorld();
    QTileSet*   createTileSet();

};

#endif // NEWWORLDWIZARD_H
