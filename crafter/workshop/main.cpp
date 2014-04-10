#include "mainwindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Crafter Workshop");
    app.setOrganizationName("JB Soft");

    QSettings::setDefaultFormat(QSettings::IniFormat);

    Q_INIT_RESOURCE(crafter);

    MainWindow w;
    w.show();

    return app.exec();
}
