#include "yassyntaxhighlighter.h"

YasSyntaxHighlighter::YasSyntaxHighlighter(QTextDocument *parent):
    QSyntaxHighlighter(parent),
    mRules(),
    mKeywordFormat(),
    mTypeFormat(),
    mSingleLineCommentFormat(),
    mMultiLineCommentFormat()
{
    HighlightingRule rule;

    mTypeFormat.setForeground(Qt::darkMagenta);

    mKeywordFormat.setForeground(Qt::darkBlue);
    mKeywordFormat.setFontWeight(QFont::Bold);

    QStringList typePatterns;
    foreach (const QString &pattern, typePatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = mTypeFormat;
        mRules.append(rule);
    }

    QStringList keywordPatterns;
    keywordPatterns << "\\bpackage\\b" << "\\buse\\b" << "\\bextends\\b"
                    << "\\bclass\\b" << "\\binterface\\b" << "\\bimplements\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bproperty\\b" << "\\breturn\\b" << "\\bif\\b"
                    << "\\bswitch\\b" << "\\bcase\\b" << "\\bstatic\\b"
                    << "\\bfor\\b" << "\\bwhile\\b" << "\\bforeach\\b"
                    << "\\bbreak\\b" << "\\bcontinue\\b" << "\\bnull\\b"
                    << "\\bboolean\\b" << "\\bchar\\b" << "\\bint\\b"
                    << "\\breal\\b" << "\\bstring\\b" << "\\bvoid\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = mKeywordFormat;
        mRules.append(rule);
    }

    mSingleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = mSingleLineCommentFormat;
    mRules.append(rule);

    mMultiLineCommentFormat.setForeground(Qt::darkGreen);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void YasSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, mRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, mMultiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
