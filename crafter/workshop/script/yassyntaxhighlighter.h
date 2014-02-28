#ifndef YASHIGHLIGHTER_H
#define YASHIGHLIGHTER_H

#include <QTextCharFormat>
#include <QSyntaxHighlighter>
#include <QVector>

class YasSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    YasSyntaxHighlighter(QTextDocument *parent = 0);


    virtual void highlightBlock(const QString &text);

private:

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    typedef QVector<HighlightingRule> HighlightingRules;

  // data
    HighlightingRules mRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat mKeywordFormat;
    QTextCharFormat mSingleLineCommentFormat;
    QTextCharFormat mMultiLineCommentFormat;
};

#endif // YASHIGHLIGHTER_H
