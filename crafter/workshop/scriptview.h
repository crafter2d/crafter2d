#ifndef SCRIPTVIEW_H
#define SCRIPTVIEW_H

#include <QTextEdit>

class YasSyntaxHighlighter;

namespace Ui {
class ScriptView;
}

class ScriptFile;

class ScriptView : public QTextEdit
{
    Q_OBJECT

public:
    explicit ScriptView(QWidget *parent = 0);
    ~ScriptView();

  // operations
    void setScript(ScriptFile& script);

private:

  // get/set
    void setHighlighter(YasSyntaxHighlighter *phighlighter);

  // data
    Ui::ScriptView *ui;
    YasSyntaxHighlighter* mpHighlighter;

};

#endif // SCRIPTVIEW_H
