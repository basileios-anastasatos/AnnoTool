#ifndef ANNOFILEDATA_H_
#define ANNOFILEDATA_H_

#include <QList>
#include <QUuid>
#include "AnnoImageInfo.h"
#include "AnnoInfo.h"
#include "Annotation.h"
#include "AllAnnoExceptions.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;

        class AnnoFileData {
            private:
                QString _sourceFile;
                AnnoImageInfo _imageInfo;
                AnnoInfo _annoInfo;
                QList<Annotation *> _annoList;

            private:
                void loadFromFile() throw(IOException *, XmlException *);
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);

            public:
                AnnoFileData(const QString &path);
                virtual ~AnnoFileData();

            public:
                AnnoImageInfo *imageInfo();
                AnnoInfo *annoInfo();
                QList<Annotation *> *annoList();
                QString filePath() const;
                void setFilePath(const QString &path);

            public:
                bool containsAnnotation(const QUuid &uuid) const;
                Annotation *getAnnotation(const QUuid &uuid);
                QUuid imageUuid() const;
                QString imageUuidAsString() const;
                QUuid complexUuid() const;
                QString complexUuidAsString() const;

            public:
                void print() const;

            public:
                void writeToFile() const throw(IOException *, XmlException *);
                static AnnoFileData *fromFile(const QString &path) throw(IOException *,
                        XmlException *);

                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoFileData *fromXml(QXmlStreamReader &reader)
                throw(XmlException *);

                static bool probeFile(const QString &file, const QUuid &uuid) throw(IOException *);
        };

    } //end namespace dt
} //end namespace anno

#endif /*ANNOFILEDATA_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
