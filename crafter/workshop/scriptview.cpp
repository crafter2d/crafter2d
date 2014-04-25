#include "scriptview.h"
#include "ui_scriptview.h"

#include <QTextDocument>
#include <QFont>
#include <QPainter>

#include "script/yassyntaxhighlighter.h"
#include "script/scriptlinenumberarea.h"
#include "script/scriptfile.h"

ScriptView::ScriptView(QWidget* parent):
    QPlainTextEdit(parent),
    ui(new Ui::ScriptView),
    mpScriptFile(nullptr),
    mpHighlighter(nullptr),
    mpLineNumberArea(nullptr),
    mCountCache(),
    mTabWidth(0)
{
    ui->setupUi(this);

    installFont();
    installHighlighter();
    installLineNumberArea();

    updateLineNuberAreaWidth(0);
}

ScriptView::ScriptView(ScriptFile& script) :
    QPlainTextEdit(nullptr),
    ui(new Ui::ScriptView),
    mpScriptFile(&script),
    mpHighlighter(nullptr),
    mpLineNumberArea(nullptr),
    mCountCache(),
    mTabWidth(0)
{
    ui->setupUi(this);

    script.ensureLoaded();
    QTextDocument& doc = script.getDocument();   
    setDocument(&doc);

    setWindowTitle(script.getPath());

    installFont();
    installHighlighter();
    installLineNumberArea();
}

ScriptView::~ScriptView()
{
    delete mpHighlighter;
    delete ui;
}

// - Get/set

bool ScriptView::hasScriptFile() const
{
    return mpScriptFile != nullptr;
}

ScriptFile& ScriptView::getScriptFile()
{
    Q_ASSERT(hasScriptFile());
    return *mpScriptFile;
}

// - Query

int ScriptView::currentLine() const
{
    return textCursor().blockNumber() + 1;
}

int ScriptView::getLineNumberAreaWidth() const
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while ( max >= 10 )
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

// - Overrides

void ScriptView::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    QRect area(cr.left(), cr.top(), getLineNumberAreaWidth(), cr.height());
    mpLineNumberArea->setGeometry(area);
}

// - Operations

void ScriptView::installFont()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);

    const int tabstop = 3;
    QFontMetrics metrics(font);
    mTabWidth = metrics.width(' ') * tabstop;

    setTabStopWidth(mTabWidth);
}

void ScriptView::installHighlighter()
{
    delete mpHighlighter;
    mpHighlighter = new YasSyntaxHighlighter(document());
}

void ScriptView::installLineNumberArea()
{
    mpLineNumberArea = new ScriptLineNumberArea(*this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNuberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    updateLineNuberAreaWidth(0);
}

void ScriptView::updateLineNuberAreaWidth(int)
{
    setViewportMargins(getLineNumberAreaWidth(), 0, 0, 0);
}

void ScriptView::updateLineNumberArea(const QRect& rect, int dy)
{
    if ( dy != 0 )
    {
        mpLineNumberArea->scroll(0, dy);
    }
    else if ( mCountCache.first != blockCount()
           || mCountCache.second != textCursor().block().lineCount() )
    {
        mpLineNumberArea->update(0, rect.y(), mpLineNumberArea->width(), rect.height());
        mCountCache.first = blockCount();
        mCountCache.second = textCursor().block().lineCount();
    }

    if ( rect.contains(viewport()->rect()) )
    {
        updateLineNuberAreaWidth(0);
    }
}

void ScriptView::paintLineNumberArea(QPaintEvent* pevent)
{
    QPainter painter(mpLineNumberArea);

    QTextBlock block = firstVisibleBlock();
    int blocknumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while ( block.isValid() && top <= pevent->rect().bottom() )
    {
        if ( block.isVisible() && bottom >= pevent->rect().top())
        {
            QString number = QString::number(blocknumber + 1);
            painter.setPen(Qt::darkGray);
            painter.drawText(0, top, mpLineNumberArea->width(),
                             fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) + blockBoundingRect(block).height();
        ++blocknumber;
    }
}

// - Slots

void ScriptView::setCurrentLine(int linenr)
{
    QTextCursor cursor(textCursor());

    int move = linenr - currentLine();
    if ( move != 0 )
    {
        if ( move < 0 )
            cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, move);
        else
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, move);

        cursor.movePosition(QTextCursor::StartOfLine);

        setTextCursor(cursor);
    }
}

