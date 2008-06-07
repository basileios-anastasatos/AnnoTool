#ifndef ANNOCLASSLIST_H_
#define ANNOCLASSLIST_H_

#include <QList>
#include "AnnoClassDefinition.h"
#include "AllAnnoExceptions.h"

//namespace AnnoTool
namespace anno {

    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;
        class AnnoClassList {
            private:
                QString _sourceFile;
                QList<AnnoClassDefinition *> _classes;

            private:
                void loadFromFile() throw(IOException *, XmlException *);
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);

            public:
                AnnoClassList(const QString &file);
                virtual ~AnnoClassList();

            public:
                bool contains(const QString &name) const;
                void add(AnnoClassDefinition *anClass) throw(NameConflictException *);
                void remove(const QString &name);
                AnnoClassDefinition *getClass(const QString &name) const
                throw(NoSuchElementException *);
                AnnoClassDefinition *getClass(int index) const
                throw(OutOfRangeException *);
                QList<QString> getClassNames() const;
                int classCount() const;
                QString filePath() const;
                void setFilePath(const QString &path);

            public:
                void print() const;

            public:
                void writeToFile() const throw(IOException *, XmlException *);
                static AnnoClassList *fromFile(const QString &path)
                throw(IOException *, XmlException *);

                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoClassList *fromXml(QXmlStreamReader &reader)
                throw(IOException *, XmlException *);
        };

    } //end namespace dt
} //end namespace anno

#endif /*ANNOCLASSLIST_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
