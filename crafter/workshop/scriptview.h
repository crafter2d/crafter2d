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
             ScriptView(QWidget *parent = 0);
    explicit ScriptView(ScriptFile &script);
    virtual ~ScriptView();

private:

  // operations
    void installFont();
    void installHighlighter();

  // data
    Ui::ScriptView *ui;
    YasSyntaxHighlighter* mpHighlighter;

};

#endif // SCRIPTVIEW_H
