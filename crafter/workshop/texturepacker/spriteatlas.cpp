#include "spriteatlas.h"

#include <QDir>
#include <QMessageBox>

#include "spritesheetreader.h"
#include "spritesheetwriter.h"

SpriteAtlas::SpriteAtlas():
    mSheets(),
    mWatcher()
{
    connect(&mWatcher, SIGNAL(directoryChanged(QString)), SLOT(on_directoryChanged(QString)));
}

int SpriteAtlas::lookup(const QString& spritename) const
{
    for ( int index = 0; index < mSheets.size(); index++ )
    {
        auto& sheet = mSheets[index];
        int result = sheet.lookup(spritename);
        if ( result != SpriteSheet::INVALID_INDEX )
        {
            result |= (index << 16);
            return result;
        }
    }
    return SpriteSheet::INVALID_INDEX;
}

QRect SpriteAtlas::bounds(int index)
{
    if ( index != SpriteSheet::INVALID_INDEX )
    {
        int sheetidx = index >> 16;
        return mSheets[sheetidx].bounds(index & 0xffff);
    }
    return QRect();
}

void SpriteAtlas::paint(QPainter& painter, const QPoint& pos, int index)
{
    int sheetidx = index >> 16;
    mSheets[sheetidx].paint(painter, pos, index & 0xffff);
}

void SpriteAtlas::append(SpriteSheet&& sheet)
{
    mSheets.append(qMove(sheet));
}

void SpriteAtlas::clear()
{
    mSheets.clear();
}

void SpriteAtlas::load(const QString& path)
{
    SpriteAtlasReader::read(path, *this);
}

void SpriteAtlas::save(const QString& path)
{
    SpriteAtlasWriter::write(path, *this);
}

// - Slots

void SpriteAtlas::on_directoryChanged(const QString& path)
{
}
