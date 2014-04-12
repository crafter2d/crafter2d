#include "scriptlinenumberarea.h"

#include "../scriptview.h"

ScriptLineNumberArea::ScriptLineNumberArea(ScriptView &view):
    QWidget(&view),
    mView(view)
{
}


// - Overrides

QSize ScriptLineNumberArea::sizeHint() const
{
    return QSize(mView.getLineNumberAreaWidth(), 0);
}

void ScriptLineNumberArea::paintEvent(QPaintEvent *event)
{
    mView.paintLineNumberArea(event);
}
