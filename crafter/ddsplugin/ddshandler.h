#ifndef DDSHANDLER_H
#define DDSHANDLER_H

#include <QImageIOHandler>

class DDSHandler : public QImageIOHandler
{
public:
    static bool canRead(QIODevice *device);

    DDSHandler();

    virtual bool canRead() const override;
    virtual bool read(QImage *image) override;
    virtual bool write(const QImage &image) override;
};

#endif // DDSHANDLER_H
