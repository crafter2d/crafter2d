#ifndef DDSPLUGIN_H
#define DDSPLUGIN_H

#include <QImageIOPlugin>


class DDSPlugin : public QImageIOPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QImageIOHandlerFactoryInterface" FILE "DDSPlugin.json")
#endif // QT_VERSION >= 0x050000

public:
    DDSPlugin(QObject *parent = 0);

    virtual Capabilities capabilities(QIODevice *device, const QByteArray &format) const override;
    virtual QImageIOHandler *create(QIODevice *device, const QByteArray &format) const override;
};

#endif // DDSPLUGIN_H
