#include "include/RecentAttrValues.h"

RecentAttrValues::RecentAttrValues() {
}

QSet<QString> RecentAttrValues::allValues() const {
    return attrValues;
}



// vim:ts=4:sts=4:sw=4:tw=80:expandtab
