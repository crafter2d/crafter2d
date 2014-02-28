#include "scriptview.h"
#include "ui_scriptview.h"

#include <QFont>

#include "script/yassyntaxhighlighter.h"

ScriptView::ScriptView(QWidget *parent) :
    QTextEdit(parent),
    ui(new Ui::ScriptView),
    mpHighlighter(0)
{
    ui->setupUi(this);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);

    mpHighlighter = new YasSyntaxHighlighter(document());
}

ScriptView::~ScriptView()
{
    delete ui;
}
