#ifndef XMLHELPERS_H
#define XMLHELPERS_H

#include <vector>
#include <iostream>

namespace libAn {

    std::string getElementDataString(const std::string &name, const std::string &doc);
    int getElementDataInt(const std::string &name, const std::string &doc);
    float getElementDataFloat(const std::string &name, const std::string &doc);
    std::vector<std::string> getElements(const std::string &name, const std::string &doc);

} //end namespace

#endif


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
