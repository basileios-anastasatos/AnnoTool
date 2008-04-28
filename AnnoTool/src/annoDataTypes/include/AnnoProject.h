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
        class AnnoProject {
            private:
                QString _sourceFile;
                QString _projectName;
                QList<QFileInfo> _classPaths;
                QList<QUuid> _links;
                QList<QFileInfo> _searchPaths;
                QUuid _uuid;

            private:
                void loadFromFile() throw(IOException *, XmlException *);
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
                void loadClassPath(QXmlStreamReader &reader) throw(XmlException *);
                void loadSearchPath(QXmlStreamReader &reader) throw(XmlException *);
                void loadLinks(QXmlStreamReader &reader) throw(XmlException *);

            public:
                AnnoProject(const QString &path);
                AnnoProject(const QString &path, const QUuid &uuid);
                virtual ~AnnoProject();

            public:
                void addToClassPath(const QString &file);
                void addToSearchPath(const QString &dir);
                void addToLinks(const QString &uuid);
                bool containsInClassPath(const QString &file) const;
                bool containsInSearchPath(const QString &dir) const;
                bool containsInLinks(const QString &uuid) const;
                QList<QUuid> *links();
                QList<QFileInfo> *searchPath();
                QList<QFileInfo> *classPath();
                QString filePath() const;
                void setFilePath(const QString &path);
                QString projectName() const;
                void setProjectName(const QString &name);

            public:
                QUuid uuid() const;
                QString uuidAsString() const;
                void setUuid(const QUuid &uuid);

            public:
                void print() const;

            public:
                void writeToFile() const throw(IOException *, XmlException *);
                static AnnoProject *fromFile(const QString &path) throw(IOException *,
                        XmlException *);

                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoProject *fromXml(QXmlStreamReader &reader)
                throw(IOException *, XmlException *);
        };

    } //end namespace dt
} //end namespace anno
#endif /*ANNOCOMPLEX_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
