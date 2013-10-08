#include "ddsplugin.h"

#include "ddshandler.h"

DDSPlugin::DDSPlugin(QObject *parent):
    QImageIOPlugin(parent)
{
}

QImageIOPlugin::Capabilities DDSPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    if (format == "dds")
        return Capabilities(CanRead);
    if (!format.isEmpty())
        return 0;
    if (!device->isOpen())
        return 0;

    Capabilities cap;
    if (device->isReadable() && DDSHandler::canRead(device))
        cap |= CanRead;
    return cap;
}

QImageIOHandler *DDSPlugin::create(QIODevice *device, const QByteArray &format) const
{
    QImageIOHandler *handler = new DDSHandler();
    handler->setDevice(device);
    handler->setFormat(format);
    return handler;
}
