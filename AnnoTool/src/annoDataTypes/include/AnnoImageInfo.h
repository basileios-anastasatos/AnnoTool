#ifndef ANNOIMAGEINFO_H_
#define ANNOIMAGEINFO_H_

#include <QFileInfo>
#include <QUuid>
#include <QString>
#include "AllAnnoExceptions.h"

class QXmlStreamWriter;
class QXmlStreamReader;

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;
        class AnnoImageInfo {
            private:
                QFileInfo _imagePath;
                QUuid _imageId;
                QString _imageSource;
                QString _comment;

            private:
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);

            public:
                AnnoImageInfo();
                virtual ~AnnoImageInfo();

            public:
                QFileInfo imagePath() const;
                QUuid imageId() const;
                QString imageIdAsString() const;
                QString imageSource() const;
                QString comment() const;

            public:
                void setImagePath(const QString &path);
                void setImagePath(const QFileInfo &path);
                void setImageId(const QUuid &id);
                void setImageSource(const QString &source);
                void setComment(const QString &comment);

            public:
                void print() const;

            public:
                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoImageInfo fromXml(QXmlStreamReader &reader)
                throw(XmlException *);
        };

    } //end namespace dt
} //end namespace anno

#endif /*ANNOIMAGEINFO_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
