#ifndef ANNOFILEDATA_H_
#define ANNOFILEDATA_H_

#include <QObject>
#include <QList>
#include <QMap>
#include <QUuid>
#include "AnnoImageInfo.h"
#include "AnnoInfo.h"
#include "Annotation.h"
#include "AllAnnoExceptions.h"
#include "XmlHelper.h"

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;

        class AnnoFileData : public QObject {
                Q_OBJECT

                friend class AnnoImageInfo;
                friend class AnnoInfo;

                // modified handling stuff
            private:
                bool _modified;
                bool _notify;
                bool _notifyOnChange;
                bool _notifyAnno;
                bool _notifyOnChangeAnno;

                // private vars
            private:
                QString _sourceFile;
                AnnoImageInfo _imageInfo;
                AnnoInfo _annoInfo;
                QList<Annotation *> _annoList;
                QMap<QUuid, Annotation *> _annoMap;

                // internal XML stuff
            private:
                void loadFromFile() throw(IOException *, XmlException *);
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);

                // internal modified stuff
            private:
                void onAnnoInfoNotify(bool prevState, bool curState);
                void onAnnoImageInfoNotify(bool prevState, bool curState);

                // internal modified slots
            private slots:
                void onAnnoModified(::anno::dt::Annotation *anno);
                void onAnnoModifyReset(::anno::dt::Annotation *anno);
                void onAnnoModifyStateChanged(::anno::dt::Annotation *anno, bool prevState, bool curState);

                // public con/destructors
            public:
                AnnoFileData(QObject *parent, const QString &path);
                AnnoFileData(const QString &path);
                ~AnnoFileData();

                // public modify handling interface
            public:
                void setModified(bool mod = true);
                bool isModified() const;
                void setNotify(bool notify = true);
                bool notifyEnabled() const;
                void setNotifyOnChange(bool notify = true);
                bool notifyOnChangeEnabled() const;
                void setNotifyAnno(bool notify = true);
                bool notifyAnnoEnabled() const;
                void setNotifyOnAnnoChange(bool notify = true);
                bool notifyOnAnnoChangeEnabled() const;
                void resetModifiedState(bool noNotify = true);
                void setAllNotifications(bool notify);

                // public read/write access interface
            public:
                AnnoImageInfo *imageInfo();
                const AnnoImageInfo *imageInfo() const;
                AnnoInfo *annoInfo();
                const AnnoInfo *annoInfo() const;
                QList<Annotation *> annoList();
                QList<const Annotation *> annoList() const;
                QString filePath() const;
                void setFilePath(const QString &path);

                int annoCount() const;
                Annotation *getAnnotation(int index);
                const Annotation *getAnnotation(int index) const;
                Annotation *getAnnotation(const QUuid &uuid);
                const Annotation *getAnnotation(const QUuid &uuid) const;
                bool containsAnnotation(const QUuid &uuid) const;
                bool containsAnnotation(const Annotation *anno) const;
                QListIterator<Annotation *> getAnnoIterator() const;

                void addAnnotation(Annotation *anno);
                void removeAnnotation(int index);
                void removeAnnotation(const QUuid &uuid);

                QUuid imageUuid() const;
                QString imageUuidAsString() const;
                QUuid complexUuid() const;
                QString complexUuidAsString() const;

            public:
                void print() const;

                // public XML interface
            public:
                void writeToFile() const throw(IOException *, XmlException *);
                static AnnoFileData *fromFile(const QString &path) throw(IOException *,
                        XmlException *);

                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoFileData *fromXml(QXmlStreamReader &reader) throw(XmlException *);

                static bool probeFile(const QString &file, const QUuid &uuid) throw(IOException *);

            signals:
                void modified(::anno::dt::AnnoFileData *annoFile);
                void modifyReset(::anno::dt::AnnoFileData *annoFile);
                void modifyStateChanged(::anno::dt::AnnoFileData *annoFile, bool prevState, bool curState);
                void annoModified(::anno::dt::AnnoFileData *annoFile, ::anno::dt::Annotation *anno);
                void annoModifyReset(::anno::dt::AnnoFileData *annoFile, ::anno::dt::Annotation *anno);
                void annoModifyStateChanged(::anno::dt::AnnoFileData *annoFile, ::anno::dt::Annotation *anno,
                                            bool prevState, bool curState);
        };

        // inlining
        //-----------------------------------------------------------
        inline AnnoFileData::AnnoFileData(QObject *parent, const QString &path) :
            QObject(parent), _modified(false), _sourceFile(path) {
            setAllNotifications(false);
            _notifyOnChangeAnno = true;
        }

        inline AnnoFileData::AnnoFileData(const QString &path) :
            QObject(NULL), _modified(false), _sourceFile(path) {
            setAllNotifications(false);
            _notifyOnChangeAnno = true;
        }

        inline bool AnnoFileData::isModified() const {
            return _modified;
        }

        inline void AnnoFileData::setNotify(bool notify) {
            _notify = notify;
        }

        inline bool AnnoFileData::notifyEnabled() const {
            return _notify;
        }

        inline void AnnoFileData::setNotifyOnChange(bool notify) {
            _notifyOnChange = notify;
        }

        inline bool AnnoFileData::notifyOnChangeEnabled() const {
            return _notifyOnChange;
        }

        inline bool AnnoFileData::notifyAnnoEnabled() const {
            return _notifyAnno;
        }

        inline bool AnnoFileData::notifyOnAnnoChangeEnabled() const {
            return _notifyOnChangeAnno;
        }

        inline void AnnoFileData::setNotifyAnno(bool notify) {
            _notifyAnno = notify;
        }

        inline void AnnoFileData::setNotifyOnAnnoChange(bool notify) {
            _notifyOnChangeAnno = notify;
        }

        inline void AnnoFileData::setAllNotifications(bool notify) {
            _notify = notify;
            _notifyOnChange = notify;
            _notifyAnno = notify;
            _notifyOnChangeAnno = notify;
        }

        inline AnnoImageInfo *AnnoFileData::imageInfo() {
            return &_imageInfo;
        }

        inline const AnnoImageInfo *AnnoFileData::imageInfo() const {
            return &_imageInfo;
        }

        inline AnnoInfo *AnnoFileData::annoInfo() {
            return &_annoInfo;
        }

        inline const AnnoInfo *AnnoFileData::annoInfo() const {
            return &_annoInfo;
        }

        inline QList<Annotation *> AnnoFileData::annoList() {
            return QList<Annotation *>(_annoList);
        }

        inline QString AnnoFileData::filePath() const {
            return _sourceFile;
        }

        inline int AnnoFileData::annoCount() const {
            return _annoList.size();
        }

        inline void AnnoFileData::setFilePath(const QString &path) {
            _sourceFile = path;
        }

        inline QUuid AnnoFileData::imageUuid() const {
            return _imageInfo.imageId();
        }

        inline QString AnnoFileData::imageUuidAsString() const {
            return anno::helper::XmlHelper::uuidAsString(_imageInfo.imageId());
        }

        inline QUuid AnnoFileData::complexUuid() const {
            return _annoInfo.annoComplexId();
        }

        inline QString AnnoFileData::complexUuidAsString() const {
            return _annoInfo.annoComplexIdAsString();
        }

        inline Annotation *AnnoFileData::getAnnotation(int index) {
            if (index >= 0 && index < _annoList.size()) {
                return _annoList.at(index);
            }
            return NULL;
        }

        inline const Annotation *AnnoFileData::getAnnotation(int index) const {
            if (index >= 0 && index < _annoList.size()) {
                return _annoList.at(index);
            }
            return NULL;
        }

        inline Annotation *AnnoFileData::getAnnotation(const QUuid &uuid) {
            return _annoMap.value(uuid, NULL);
        }

        inline const Annotation *AnnoFileData::getAnnotation(const QUuid &uuid) const {
            return _annoMap.value(uuid, NULL);
        }

        inline bool AnnoFileData::containsAnnotation(const QUuid &uuid) const {
            return _annoMap.contains(uuid);
        }

        inline bool AnnoFileData::containsAnnotation(const Annotation *anno) const {
            return _annoMap.contains(anno->annoId());
        }

        inline QListIterator<Annotation *> AnnoFileData::getAnnoIterator() const {
            return QListIterator<Annotation *>(_annoList);
        }

        //-----------------------------------------------------------

    } //end namespace dt
} //end namespace anno

#endif /*ANNOFILEDATA_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
