#ifndef ANNOIMAGEINFO_H_
#define ANNOIMAGEINFO_H_

#include <QFileInfo>
#include <QUuid>
#include <QString>
#include "AllAnnoExceptions.h"
#include "XmlHelper.h"

#ifndef NOFRAME
#define NOFRAME -1
#endif

class QXmlStreamWriter;
class QXmlStreamReader;

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;
        class AnnoFileData;

        class AnnoImageInfo {
                // modified handling stuff
            private:
                bool _modified;
                AnnoFileData *_parFile;

                // private vars
            private:
                QFileInfo _imagePath;
                int _frame;
                QUuid _imageId;
                QString _imageSource;
                QString _comment;

                // internal XML stuff
            private:
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);

                // public con/destructors
            public:
                AnnoImageInfo(AnnoFileData *parFile = NULL);
                ~AnnoImageInfo();

                // public modify handling interface
            public:
                void setParentFile(AnnoFileData *parFile);
                AnnoFileData *parentFile() const;
                void setModified(bool mod = true);
                bool isModified() const;

                // public read access interface
            public:
                QFileInfo imagePath() const;
                int frame() const;
                QUuid imageId() const;
                QString imageIdAsString() const;
                QString imageSource() const;
                QString comment() const;

                // public write access interface
            public:
                void setImagePath(const QString &path);
                void setImagePath(const QFileInfo &path);
                void setFrame(int frame);
                void setImageId(const QUuid &id);
                void setImageSource(const QString &source);
                void setComment(const QString &comment);

                // various methods
            public:
                void print() const;

                // public XML interface
            public:
                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoImageInfo fromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

        // inlining
        //-----------------------------------------------------------
        inline AnnoImageInfo::AnnoImageInfo(AnnoFileData *parFile) :
            _modified(false), _parFile(parFile), _frame(NOFRAME) {
        }

        inline AnnoImageInfo::~AnnoImageInfo() {
        }

        inline void AnnoImageInfo::setParentFile(AnnoFileData *parFile) {
            _parFile = parFile;
        }

        inline AnnoFileData *AnnoImageInfo::parentFile() const {
            return _parFile;
        }

        inline bool AnnoImageInfo::isModified() const {
            return _modified;
        }

        inline QFileInfo AnnoImageInfo::imagePath() const {
            return _imagePath;
        }

        inline int AnnoImageInfo::frame() const {
            return _frame;
        }

        inline QUuid AnnoImageInfo::imageId() const {
            return _imageId;
        }

        inline QString AnnoImageInfo::imageIdAsString() const {
            return anno::helper::XmlHelper::uuidAsString(_imageId);
        }

        inline QString AnnoImageInfo::imageSource() const {
            return _imageSource;
        }

        inline QString AnnoImageInfo::comment() const {
            return _comment;
        }

        inline void AnnoImageInfo::setImagePath(const QFileInfo &path) {
            if(_imagePath != path) {
                _imagePath = path;
                setModified(true);
            }
        }

        inline void AnnoImageInfo::setFrame(int frame) {
            if (_frame != frame && frame >= NOFRAME) {
                _frame = frame;
                setModified(true);
            }
        }

        inline void AnnoImageInfo::setImageId(const QUuid &id) {
            if(_imageId != id) {
                _imageId = id;
                setModified(true);
            }
        }

        inline void AnnoImageInfo::setImageSource(const QString &source) {
            if(_imageSource != source) {
                _imageSource = source;
                setModified(true);
            }
        }

        inline void AnnoImageInfo::setComment(const QString &comment) {
            if(_comment != comment) {
                _comment = comment;
                setModified(true);
            }
        }
        //-----------------------------------------------------------

    } //end namespace dt
} //end namespace anno

#endif /*ANNOIMAGEINFO_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
