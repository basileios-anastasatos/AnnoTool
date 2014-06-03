#ifndef ANNOCOMPLEX_H_
#define ANNOCOMPLEX_H_

#include <QList>
#include <QMap>
#include <QFileInfo>
#include <QString>
#include <QUuid>
#include "AllAnnoExceptions.h"
#include "AnnoFilter.h"
#include "AnnoFilterManager.h"
#include "ColorFilterEntry.h"

class QXmlStreamWriter;
class QXmlStreamReader;

//namespace AnnoTool
namespace anno {

    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;
        class AnnoProject {
                // some useful constants
            private:
                static const QString XML_PROJECT;
                static const QString XML_CLASSPATH;
                static const QString XML_SEARCHPATH;
                static const QString XML_FILTERS;
                static const QString XML_SINGLEFILTER;
                static const QString XML_LINK;
                static const QString XML_COLORRULES;
                static const QString XML_GLOBALFILTERS;

            private:
                QString _sourceFile;
                QString _projectName;
                QList<QFileInfo> _classPaths;
                QList<QUuid> _links;
                QList<QFileInfo> _searchPaths;
                QUuid _uuid;
                QMap<QString, filter::AnnoFilter *> _filters;
                QList<filter::ColorFilterEntry *>   _colorRules;

            private:
                void loadFromFile() throw(IOException *, XmlException *);
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
                void loadClassPath(QXmlStreamReader &reader) throw(XmlException *);
                void loadSearchPath(QXmlStreamReader &reader) throw(XmlException *);
                void loadLinks(QXmlStreamReader &reader) throw(XmlException *);
                void loadFilters(QXmlStreamReader &reader, bool global = false) throw(XmlException *);
                void saveFilters(QXmlStreamWriter &writer, bool global = false) const throw(XmlException *);
                void loadColorRules(QXmlStreamReader &reader) throw(XmlException *);
                void saveColorRules(QXmlStreamWriter &writer, bool global = false) const throw(XmlException *);
                bool isXmlComponent(const QString &cmpName);
                bool isXmlComponent(const QStringRef &cmpName);

            public:
                void saveGlobalFilters(void) const throw(IOException *, XmlException *);

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

                const QMap<QString, filter::AnnoFilter *> *filters()    const;
                QMap<QString, filter::AnnoFilter *>       *filters();
                const QList<filter::ColorFilterEntry *>   *colorRules() const;
                QList<filter::ColorFilterEntry *>         *colorRules();

            public:
                QUuid uuid() const;
                QString uuidAsString() const;
                void setUuid(const QUuid &uuid);

            public:
                void print() const;

            public:
                void writeToFile() const throw(IOException *, XmlException *);
                static AnnoProject *fromFile(const QString &path, filter::AnnoFilterManager *filterMan, bool globalFilters = true)
                    throw(IOException *, XmlException *);
                void loadGlobalFilters(void) throw(IOException *, XmlException *);

                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoProject *fromXml(QXmlStreamReader &reader)
                throw(IOException *, XmlException *);
        };

    } //end namespace dt
} //end namespace anno
#endif /*ANNOCOMPLEX_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
