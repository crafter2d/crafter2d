#include "scriptview.h"
#include "ui_scriptview.h"

#include <QTextDocument>
#include <QFont>

#include "script/yassyntaxhighlighter.h"
#include "script/scriptfile.h"

ScriptView::ScriptView(QWidget* parent):
    QTextEdit(parent),
    ui(new Ui::ScriptView),
    mpHighlighter(0)
{
    ui->setupUi(this);

    installFont();
    installHighlighter();
}

ScriptView::ScriptView(ScriptFile& script) :
    QTextEdit(0),
    ui(new Ui::ScriptView),
    mpHighlighter(0)
{
    ui->setupUi(this);

    script.ensureLoaded();
    QTextDocument& doc = script.getDocument();
    setDocument(&doc);

    installFont();
    installHighlighter();
}

ScriptView::~ScriptView()
{
    delete mpHighlighter;
    delete ui;
}

// - Operations

void ScriptView::installFont()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);
}

void ScriptView::installHighlighter()
{
    delete mpHighlighter;
    mpHighlighter = new YasSyntaxHighlighter(document());
}
