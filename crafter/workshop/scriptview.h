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

  // query
    int getLineNumberAreaWidth() const;

  // operations
    void paintLineNumberArea(QPaintEvent* pevent);

protected:
  // overrides
    virtual void resizeEvent(QResizeEvent *e);

private slots:
    void updateLineNuberAreaWidth(int width);
    void updateLineNumberArea(const QRect&, int);

private:

  // operations
    void installFont();
    void installHighlighter();
    void installLineNumberArea();

  // data
    Ui::ScriptView *ui;
    YasSyntaxHighlighter* mpHighlighter;
    ScriptLineNumberArea* mpLineNumberArea;
    QPair<int, int>       mCountCache;
    int mTabWidth;

};

#endif // SCRIPTVIEW_H
