#ifndef SPRITEATLAS_H
#define SPRITEATLAS_H

#include <QFileSystemWatcher>
#include <QVector>
#include <QRect>

#include "spritesheet.h"

class SpriteAtlas : public QObject
{
    Q_OBJECT

public:
    using SpriteSheets = QVector<SpriteSheet>;

    SpriteAtlas();

    int lookup(const QString& spritename) const;

    const SpriteSheets& getSheets() const {
        return mSheets;
    }

    QRect bounds(int index);

    void append(SpriteSheet &&sheet);
    void clear();

    void paint(QPainter& painter, const QPoint &pos, int index);

    void load(const QString& path);
    void save(const QString& path);

private slots:
    void on_directoryChanged(const QString& path);

private:
    friend class SpriteAtlasWriter;

  // data
    SpriteSheets mSheets;
    QFileSystemWatcher mWatcher;
};

#endif // SPRITEATLAS_H
