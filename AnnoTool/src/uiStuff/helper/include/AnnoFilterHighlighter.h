#ifndef ANNOFILTERHILIGHTER_H_
#define ANNOFILTERHILIGHTER_H_

#include <QSyntaxHighlighter>
#include <QList>

class QTextDocument;

class AnnoFilterHighlighter : public QSyntaxHighlighter {
        Q_OBJECT

    private:
        struct HighlightingRule {
            QRegExp pattern;
            QTextCharFormat format;
        };

        QList<HighlightingRule> _rules;

    private:
        void initRules();
        void initLogicOpRules();
        void initAtomRules();
        void initSpecialCharRules();

    protected:
        virtual void highlightBlock(const QString &text);

    public:
        AnnoFilterHighlighter(QTextDocument *parent = 0);
        virtual ~AnnoFilterHighlighter();


};

#endif /* ANNOFILTERHILIGHTER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
