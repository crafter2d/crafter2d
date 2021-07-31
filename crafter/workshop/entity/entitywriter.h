#ifndef ENTITYWRITER_H
#define ENTITYWRITER_H

class QIODevice;

class EntityWriter
{
public:
    explicit EntityWriter(QIODevice &device);

private:

    QIODevice& mDevice;
};

#endif // ENTITYWRITER_H
