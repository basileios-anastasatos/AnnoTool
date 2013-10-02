#ifndef RECENTATTRVALUES_H_
#define RECENTATTRVALUES_H_

#include <QSet>
#include <QString>

class RecentAttrValues {
    public:
        QSet<QString> attrNames;
        QSet<QString> attrValues;

    public:
        RecentAttrValues();

        QSet<QString> allValues() const;
};

#endif /* RECENTATTRVALUES_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
