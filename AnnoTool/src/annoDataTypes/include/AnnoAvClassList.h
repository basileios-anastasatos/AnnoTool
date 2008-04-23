#ifndef ANNOAVCLASSLIST_H_
#define ANNOAVCLASSLIST_H_

#include "AnnoClassDefinition.h"

namespace anno {

    namespace dt {

        class AnnoAvClassList {
            private:
                QList<AnnoClassDefinition *> _classes;

            public:
                AnnoAvClassList();
                virtual ~AnnoAvClassList();

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

            public:
                void print() const;

            public:
                void addFromFile(const QString &path) throw(IOException *,
                        XmlException *);
                void addFromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

    }

}

#endif /*ANNOAVCLASSLIST_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
