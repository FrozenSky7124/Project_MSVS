#include "MySyntaxHighlighter.h"
#include <QDebug>


MySyntaxHighlighter::MySyntaxHighlighter(QTextDocument * parent)
    :QSyntaxHighlighter(parent)
{

}

MySyntaxHighlighter::~MySyntaxHighlighter()
{

}

void MySyntaxHighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat charFormat;
    charFormat.setFont(QFont("JetBrains Mono NL"));
    charFormat.setFontWeight(QFont::Bold);
    charFormat.setForeground(Qt::green);

    QString pattern = "\\bchar\\b";
    QRegExp express(pattern);
    int index = text.indexOf(express);
    qDebug() << "index:" << index;
    while (index >= 0)
    {
        int matchLen = express.matchedLength();
        setFormat(index, matchLen, charFormat);
        index = text.indexOf(express, index + matchLen);
    }
    
}