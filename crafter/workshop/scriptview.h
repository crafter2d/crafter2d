#ifndef SCRIPTVIEW_H
#define SCRIPTVIEW_H

#include <QTextEdit>

class YasSyntaxHighlighter;

namespace Ui {
class ScriptView;
}

class ScriptView : public QTextEdit
{
    Q_OBJECT

public:
    explicit ScriptView(QWidget *parent = 0);
    ~ScriptView();

private:

  // data
    Ui::ScriptView *ui;
    YasSyntaxHighlighter* mpHighlighter;

};

#endif // SCRIPTVIEW_H
