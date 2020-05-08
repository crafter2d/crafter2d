#ifndef SCRIPTVIEW_H
#define SCRIPTVIEW_H

#include <QPlainTextEdit>
#include <QPair>

class ScriptLineNumberArea;
class YasSyntaxHighlighter;

namespace Ui {
class ScriptView;
}

class ScriptFile;

class ScriptView : public QPlainTextEdit
{
    Q_OBJECT

public:
             ScriptView(QWidget *parent = 0);
    explicit ScriptView(ScriptFile &script);
    virtual ~ScriptView();

  // get/set
    bool hasScriptFile() const;
    ScriptFile& getScriptFile();

  // query
    int currentLine() const;
    int getLineNumberAreaWidth() const;

  // operations
    void paintLineNumberArea(QPaintEvent* pevent);

public slots:
    void setCurrentLine(int linenr);

protected:
  // overrides
    virtual void resizeEvent(QResizeEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);

private slots:
    void updateLineNuberAreaWidth(int width);
    void updateLineNumberArea(const QRect&, int);

private:

  // operations
    void installFont();
    void installHighlighter();
    void installLineNumberArea();

    void autoIndent();
    void autoCloseBlock();

  // data
    Ui::ScriptView *ui;
    ScriptFile*           mpScriptFile;
    YasSyntaxHighlighter* mpHighlighter;
    ScriptLineNumberArea* mpLineNumberArea;
    QPair<int, int>       mCountCache;
    int                   mTabWidth;
};

#endif // SCRIPTVIEW_H
