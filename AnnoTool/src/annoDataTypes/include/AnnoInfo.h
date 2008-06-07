#ifndef ANNOINFO_H_
#define ANNOINFO_H_

#include <QUuid>
#include "AllAnnoExceptions.h"
#include "XmlHelper.h"

class QXmlStreamReader;
class QXmlStreamWriter;

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;
        class AnnoFileData;

        class AnnoInfo {
                // modified handling stuff
            private:
                bool _modified;
                AnnoFileData *_parFile;

                // private vars
            private:
                QUuid _annoComplex;

                // internal XML stuff
            private:
                void loadFromXml(QXmlStreamReader &reader) throw(XmlFormatException *);

                // public con/destructors
            public:
                AnnoInfo(AnnoFileData *parFile = NULL);
                AnnoInfo(const QUuid &uuid);
                AnnoInfo(AnnoFileData *parFile, const QUuid &uuid);
                ~AnnoInfo();

                // public modify handling interface
            public:
                void setParentFile(AnnoFileData *parFile);
                AnnoFileData *parentFile() const;
                void setModified(bool mod = true);
                bool isModified() const;

                // public member access interface
            public:
                void setComplexId(const QUuid &uuid);
                QUuid annoComplexId() const;
                QString annoComplexIdAsString() const;

                // various methods
            public:
                void print() const;

                // public XML interface
            public:
                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoInfo fromXml(QXmlStreamReader &reader) throw(XmlFormatException *);
        };

        // inlining
        //-----------------------------------------------------------
        inline AnnoInfo::AnnoInfo(AnnoFileData *parFile) :
            _modified(false), _parFile(parFile) {
        }

        inline AnnoInfo::AnnoInfo(const QUuid &uuid) :
            _modified(false), _parFile(NULL), _annoComplex(uuid) {
        }

        inline AnnoInfo::AnnoInfo(AnnoFileData *parFile, const QUuid &uuid) :
            _modified(false), _parFile(parFile), _annoComplex(uuid) {
        }

        inline AnnoInfo::~AnnoInfo() {
        }

        inline void AnnoInfo::setParentFile(AnnoFileData *parFile) {
            _parFile = parFile;
        }

        inline AnnoFileData *AnnoInfo::parentFile() const {
            return _parFile;
        }

        inline bool AnnoInfo::isModified() const {
            return _modified;
        }

        inline void AnnoInfo::setComplexId(const QUuid &uuid) {
            bool m = _annoComplex != uuid;
            _annoComplex = uuid;
            setModified(m);
        }

        inline QUuid AnnoInfo::annoComplexId() const {
            return _annoComplex;
        }

        inline QString AnnoInfo::annoComplexIdAsString() const {
            return anno::helper::XmlHelper::uuidAsString(_annoComplex);
        }
        //-----------------------------------------------------------

    } //end namespace dt
} //end namespace anno

#endif /*ANNOINFO_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
