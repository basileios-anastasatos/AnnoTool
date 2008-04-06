#ifndef ANNOCOMPLEX_H_
#define ANNOCOMPLEX_H_

#include <QList>
#include <QFileInfo>
#include <QString>
#include <QUuid>
#include "AllAnnoExceptions.h"

class QXmlStreamWriter;
class QXmlStreamReader;

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;
        class AnnoComplex {
            private:
                QList<QFileInfo> _classPaths;
                QList<QUuid> _links;
                QList<QFileInfo> _searchPaths;
                QUuid _uuid;

            private:
                void loadClassPath(QXmlStreamReader &reader) throw(XmlException *);
                void loadSearchPath(QXmlStreamReader &reader) throw(XmlException *);
                void loadLinks(QXmlStreamReader &reader) throw(XmlException *);

            public:
                AnnoComplex();
                AnnoComplex(const QUuid &uuid);
                virtual ~AnnoComplex();

                void addToClassPath(const QString &file);
                void addToSearchPath(const QString &dir);
                void addToLinks(const QString &uuid);
                bool containsInClassPath(const QString &file) const;
                bool containsInSearchPath(const QString &dir) const;
                bool containsInLinks(const QString &uuid) const;
                const QList<QUuid> *links();
                const QList<QFileInfo> *searchPath();
                const QList<QFileInfo> *classPath();

                void print() const;


                QUuid uuid() const;
                QString uuidAsString() const;
                static QString uuidAsString(const QUuid &uuid);
                void setUuid(const QUuid &uuid);

                static AnnoComplex loadFromFile(const QString &path) throw(IOException *,
                        XmlException *);
                void writeToFile(const QString &path) const throw(IOException *, XmlFormatException *);
                void toXml(QXmlStreamWriter &writer) const throw(XmlFormatException *);
        };

    } //end namespace dt
} //end namespace anno
#endif /*ANNOCOMPLEX_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
