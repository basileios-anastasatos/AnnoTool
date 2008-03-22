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
                bool skipTo(QString tagName, QXmlStreamReader &reader)
                throw(XmlException *);

            public:
                AnnoClassList(QString file);
                virtual ~AnnoClassList();

            public:
                bool contains(QString name) const;
                void add(AnnoClassDefinition *anClass) throw(NameConflictException *);
                void remove(QString name);
                AnnoClassDefinition *getClass(QString name) const
                throw(NoSuchElementException *);
                AnnoClassDefinition *getClass(int index) const
                throw(OutOfRangeException *);
                QList<QString> getClassNames() const;
                int classCount() const;

            public:
                void loadFromFile() throw(IOException *, XmlException *);
                void writeToFile() const throw(IOException *);

            public:
                void print() const;

            public:
                static AnnoClassList fromXml(QString file) throw(IOException *,
                        XmlException *);
        };

    } //end namespace dt
} //end namespace anno

#endif /*ANNOCLASSLIST_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
