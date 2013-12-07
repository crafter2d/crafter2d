#include "mainwindow.h"
#include <QApplication>
#include <QSettings>

#include <core/physics/simulationfactoryregistry.h>
#include <engine/physics/box2d/box2dfactory.h>

int main(int argc, char *argv[])
{
    SimulationFactoryRegistry::getInstance().addFactory(new Box2DFactory());

    QApplication app(argc, argv);
    app.setApplicationName("Crafter Workshop");
    app.setOrganizationName("JB Soft");

    QSettings::setDefaultFormat(QSettings::IniFormat);

    Q_INIT_RESOURCE(crafter);

    MainWindow w;
    w.show();

    return app.exec();
}
