#include "include/AnnoFilterHighlighter.h"
#include "AllAnnoFilterRules.h"
#include "importGlobals.h"


AnnoFilterHighlighter::AnnoFilterHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent) {
    initRules();
}

AnnoFilterHighlighter::~AnnoFilterHighlighter() {
    // nothing to do.
}

void AnnoFilterHighlighter::initRules() {
    initSpecialCharRules();
    initLogicOpRules();
    initAtomRules();


}

void AnnoFilterHighlighter::initLogicOpRules() {
    QTextCharFormat format;
    format.setForeground(QBrush(QColor(0, 0, 235, 255)));
    format.setFontWeight(QFont::Bold);
    HighlightingRule rule;
    rule.format = format;

    rule.pattern = QRegExp(QString("\\b%1\\b").arg(anno::filter::AfrAnd::xmlName()));
    _rules.append(rule);
    rule.pattern = QRegExp(QString("\\b%1\\b").arg(anno::filter::AfrOr::xmlName()));
    _rules.append(rule);
    rule.pattern = QRegExp(QString("\\b%1\\b").arg(anno::filter::AfrXor::xmlName()));
    _rules.append(rule);
    rule.pattern = QRegExp(QString("\\b%1\\b").arg(anno::filter::AfrNot::xmlName()));
    _rules.append(rule);
}

void AnnoFilterHighlighter::initAtomRules() {
    QTextCharFormat format;
    format.setForeground(QBrush(QColor(0, 0, 170, 255)));
    format.setFontWeight(QFont::Bold);
    HighlightingRule rule;
    rule.format = format;

    rule.pattern = QRegExp(QString("\\b%1\\b").arg(anno::filter::AfrAttributeValue::xmlName()));
    _rules.append(rule);
    rule.pattern = QRegExp(QString("\\b%1\\b").arg(anno::filter::AfrClass::xmlName()));
    _rules.append(rule);
    rule.pattern = QRegExp(QString("\\b%1\\b").arg(anno::filter::AfrScore::xmlName()));
    _rules.append(rule);
//  rule.pattern = QRegExp(QString("\\b%1\\b").arg(anno::filter::AfrSwitch::xmlName()));
//  _rules.append(rule);
    rule.pattern = QRegExp(QString("\\b%1\\b").arg(anno::filter::AfrShape::xmlName()));
    _rules.append(rule);
}

void AnnoFilterHighlighter::initSpecialCharRules() {
    QTextCharFormat format;
    format.setForeground(Qt::black);
    format.setFontWeight(QFont::Bold);
    HighlightingRule rule;
    rule.format = format;

    rule.pattern = QRegExp("([></])");
    _rules.append(rule);
}

void AnnoFilterHighlighter::highlightBlock(const QString &text) {
    foreach (HighlightingRule rule, _rules) {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
