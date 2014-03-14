#include "scriptview.h"
#include "ui_scriptview.h"

#include <QFont>
#include <QFile>

#include "script/yassyntaxhighlighter.h"
#include "script/scriptfile.h"

ScriptView::ScriptView(QWidget *parent) :
    QTextEdit(parent),
    ui(new Ui::ScriptView),
    mpHighlighter(0)
{
    ui->setupUi(this);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(11);
    setFont(font);

    mpHighlighter = new YasSyntaxHighlighter(document());
}

ScriptView::~ScriptView()
{
    delete ui;
    delete mpHighlighter;
}

// - Get/set

void ScriptView::setHighlighter(YasSyntaxHighlighter* phighlighter)
{
    delete mpHighlighter;
    mpHighlighter = phighlighter;
}

// - Operations

void ScriptView::setScript(ScriptFile& script)
{
    QFile file(script.getPath());

    if ( file.open(QIODevice::ReadOnly) )
    {
        QString data(file.readAll());

        QTextDocument* pdoc = new QTextDocument();
        pdoc->setPlainText(data);
        setDocument(pdoc);

        setHighlighter(new YasSyntaxHighlighter(document()));
    }
}
