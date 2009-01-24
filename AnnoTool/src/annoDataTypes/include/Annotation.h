#ifndef ANNOTATION_H_
#define ANNOTATION_H_

#include <QObject>
#include <QUuid>
#include <QList>
#include <QSet>
#include <cmath>
#include "AllAnnoExceptions.h"
#include "AnnoShape.h"
#include "XmlHelper.h"

#include "GlobalLogger.h"
using logging::GlobalLogger;

#ifndef NATIVE_SCORE_ATTR
#define NATIVE_SCORE_ATTR QString("__score")
#endif

#ifndef NATIVE_ZORDER_ATTR
#define NATIVE_ZORDER_ATTR QString("__zorder")
#endif

#ifndef NATIVE_CLASS_POSEPOINT
#define NATIVE_CLASS_POSEPOINT QString("PosePoint")
#endif

#ifndef NATIVE_POSEPOINT_ID_ATTR
#define NATIVE_POSEPOINT_ID_ATTR QString("id")
#endif

class QXmlStreamWriter;
class QXmlStreamReader;

//namespace AnnoTool
namespace anno {
    //namespace DataTypes
    namespace dt {
        using namespace ::anno::exc;
        class Annotation;

        class AnnoAttribute {
                // modified handling stuff
            private:
                bool _modified;
                Annotation *_parAnno;

                // member vars
            private:
                QString _name;
                QString _className;
                QString _value;

                // internal XML stuff
            private:
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);

                // public con/destructors
            public:
                AnnoAttribute(Annotation *parAnno = NULL);
                AnnoAttribute(Annotation *parAnno, const QString &name, const QString &className,
                              const QString &value);
                AnnoAttribute(const QString &name, const QString &className,
                              const QString &value);
                ~AnnoAttribute();

                // public modify handling interface
            public:
                void setParentAnno(Annotation *parAnno);
                Annotation *parentAnno() const;
                void setModified(bool mod = true);
                bool isModified() const;

                // public member access interface
            public:
                QString name() const;
                QString className() const;
                QString value() const;
                void setName(const QString &val);
                void setClassName(const QString &val);
                void setValue(const QString &val);

                // various methods
            public:
                void print() const;

                //public operators
            public:
                bool operator==(const AnnoAttribute &attr) const;

                // public XML interface
            public:
                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static AnnoAttribute fromXml(QXmlStreamReader &reader) throw(XmlException *);
        };

        class Annotation : public QObject {
                Q_OBJECT

                friend class AnnoAttribute;

                // modified handling stuff
            private:
                bool _modified;
                bool _notify;
                bool _notifyOnChange;
                bool _notifyAttr;

                // private vars
            private:
                QUuid _annoId;
                double _score;
                int _zOrder;
                QUuid _annoParent;
                QList<QUuid> _annoChildren;
                QString _comment;
                QList<QString> _annoClasses;
                QList<AnnoAttribute> _annoAttributes;
                AnnoShape *_shape;

                // internal XML stuff
            private:
                void loadFromXml(QXmlStreamReader &reader) throw(XmlException *);
                void annoHierarchyToXml(QXmlStreamWriter &writer) const throw(XmlException *);
                void annoClassesToXml(QXmlStreamWriter &writer) const throw(XmlException *);
                void annoAttributesToXml(QXmlStreamWriter &writer) const throw(XmlException *);
                void loadAnnoHierarchyFromXml(QXmlStreamReader &reader) throw(XmlException *);
                void loadAnnoClassesFromXml(QXmlStreamReader &reader) throw(XmlException *);
                void loadAnnoAttributesFromXml(QXmlStreamReader &reader) throw(XmlException *);

                // internal attribute modified stuff
            private:
                void onAttrModified(AnnoAttribute *attr);
                void onAttrModifyReset(AnnoAttribute *attr);

                // public con/destructors
            public:
                Annotation(QObject *parent = 0);
                Annotation(const Annotation *anno, QObject *parent = 0);
                virtual ~Annotation();

                // public modify handling interface
            public:
                void setModified(bool mod = true);
                bool isModified() const;
                void setNotify(bool notify = true);
                bool notifyEnabled() const;
                void setNotifyOnChange(bool notify = true);
                bool notifyOnChangeEnabled() const;
                void setNotifyAttr(bool notify = true);
                bool notifyAttrEnabled() const;
                void resetModifiedState(bool noNotify = true);
                void setAllNotifications(bool notify);

                // public member access interface
            public:
                QUuid annoId() const;
                QString annoIdAsString() const;
                void setAnnoId(const QUuid &uuid);
                void setScore(double score);
                double score() const;
                bool hasScore() const;
                bool hasAnnoParent() const;
                bool hasAnnoChildren() const;
                int annoChildCount() const;
                QUuid annoParent() const;
                bool containsAnnoChild(const QUuid &child) const;
                QList<QUuid> annoChildren() const;
                void setAnnoParent(const QUuid &parent);
                void addAnnoChild(const QUuid &child);
                void insertAnnoChild(int pos, const QUuid &child);
                void removeAnnoChild(const QUuid &child);
                void removeAnnoChild(int child);
                void removeAnnoChildAll();
                QString comment() const;
                void setComment(const QString &comment);
                AnnoShape *shape() const;
                void setShape(AnnoShape *shape);
                QList<QString> classes() const;
                QList<AnnoAttribute> attributes() const;

                void addClass(const QString &val);
                void addAttribute(AnnoAttribute attr);
                int classCount() const;
                int attributeCount() const;
                QString getClass(int index) const;
                AnnoAttribute *getAttribute(int index);
                const AnnoAttribute *getAttribute(int index) const;
                void removeClass(int index);
                void removeClass(const QString &val);
                void removeClassAll();
                void removeAttribute(int index);
                void removeAttributeAll();

                /** MA: helpers */
                bool getClassAttributeValue(QString qsClass, QString qsAttribute, QString &qsValue);

                // various methods
            public:
                void print() const;
                QString annoInfo() const;

                // public XML interface
            public:
                void toXml(QXmlStreamWriter &writer) const throw(XmlException *);
                static Annotation *fromXml(QXmlStreamReader &reader) throw(XmlException *);

            signals:
                void modified(::anno::dt::Annotation *anno);
                void modifyReset(::anno::dt::Annotation *anno);
                void modifyStateChanged(::anno::dt::Annotation *anno, bool prevState,
                                        bool curState);
                void attributeModified(::anno::dt::Annotation *anno,
                                       ::anno::dt::AnnoAttribute *attr);
                void attributeModifyReset(::anno::dt::Annotation *anno,
                                          ::anno::dt::AnnoAttribute *attr);

        };

        // inlining
        //-----------------------------------------------------------
        inline AnnoAttribute::~AnnoAttribute() {
        }

        inline void AnnoAttribute::setParentAnno(Annotation *parAnno) {
            _parAnno = parAnno;
        }

        inline Annotation *AnnoAttribute::parentAnno() const {
            return _parAnno;
        }

        inline bool AnnoAttribute::isModified() const {
            return _modified;
        }

        inline QString AnnoAttribute::name() const {
            return _name;
        }

        inline QString AnnoAttribute::className() const {
            return _className;
        }

        inline QString AnnoAttribute::value() const {
            return _value;
        }

        inline void AnnoAttribute::setName(const QString &val) {
            if (_name != val) {
                _name = val;
                setModified(true);
            }
        }

        inline void AnnoAttribute::setClassName(const QString &val) {
            if (_className != val) {
                _className = val;
                setModified(true);
            }
        }

        inline void AnnoAttribute::setValue(const QString &val) {
            if (_value != val) {
                _value = val;
                setModified(true);
            }
        }

        inline bool Annotation::isModified() const {
            return _modified;
        }

        inline void Annotation::setNotify(bool notify) {
            _notify = notify;
        }

        inline bool Annotation::notifyEnabled() const {
            return _notify;
        }

        inline void Annotation::setNotifyOnChange(bool notify) {
            _notifyOnChange = notify;
        }

        inline bool Annotation::notifyOnChangeEnabled() const {
            return _notifyOnChange;
        }

        inline void Annotation::setNotifyAttr(bool notify) {
            _notifyAttr = notify;
        }

        inline bool Annotation::notifyAttrEnabled() const {
            return _notifyAttr;
        }

        inline void Annotation::setAllNotifications(bool notify) {
            _notify = notify;
            _notifyOnChange = notify;
            _notifyAttr = notify;
        }

        inline QUuid Annotation::annoId() const {
            return _annoId;
        }

        inline QString Annotation::annoIdAsString() const {
            return anno::helper::XmlHelper::uuidAsString(_annoId);
        }

        inline double Annotation::score() const {
            return _score;
        }

        inline bool Annotation::hasScore() const {
            return !std::isnan(_score);
        }

        inline bool Annotation::hasAnnoParent() const {
            return !_annoParent.isNull();
        }

        inline bool Annotation::hasAnnoChildren() const {
            return !_annoChildren.isEmpty();
        }

        inline int Annotation::annoChildCount() const {
            return _annoChildren.size();
        }

        inline QUuid Annotation::annoParent() const {
            return _annoParent;
        }

        inline bool Annotation::containsAnnoChild(const QUuid &child) const {
            return _annoChildren.contains(child);
        }

        inline QList<QUuid> Annotation::annoChildren() const {
            return _annoChildren;
        }

        inline void Annotation::setAnnoParent(const QUuid &parent) {
            if (_annoParent != parent) {
                _annoParent = parent;
                setModified(true);
            }
        }

        inline void Annotation::addAnnoChild(const QUuid &child) {
            if(!_annoChildren.contains(child)) {
                _annoChildren.append(child);
                setModified(true);
            }
        }

        inline void Annotation::insertAnnoChild(int pos, const QUuid &child) {
            if(!_annoChildren.contains(child)) {
                _annoChildren.insert(pos, child);
                setModified(true);
            }
        }

        inline void Annotation::removeAnnoChild(const QUuid &child) {
            int idx = _annoChildren.indexOf(child);
            if(idx >= 0) {
                _annoChildren.removeAt(idx);
                setModified(true);
            }
        }

        inline void Annotation::removeAnnoChild(int child) {
            if(child >= 0 && child < _annoChildren.size()) {
                _annoChildren.removeAt(child);
                setModified(true);
            }
        }

        inline void Annotation::removeAnnoChildAll() {
            if(_annoChildren.size() > 0) {
                _annoChildren.clear();
                setModified(true);
            }
        }

        inline QString Annotation::comment() const {
            return _comment;
        }

        inline AnnoShape *Annotation::shape() const {
            return _shape;
        }

        inline QList<QString> Annotation::classes() const {
            return QList<QString>(_annoClasses);
        }

        inline QList<AnnoAttribute> Annotation::attributes() const {
            //TODO überlege: bei kopie parent auf null setzen?!
            return QList<AnnoAttribute>(_annoAttributes);
        }

        inline void Annotation::setAnnoId(const QUuid &uuid) {
            if (_annoId != uuid) {
                _annoId = uuid;
                setModified(true);
            }
        }

        inline void Annotation::setScore(double score) {
            //TODO Think of suitable delta
            if (_score - score != 0) {
                _score = score;
                setModified(true);
            }
        }

        inline void Annotation::setComment(const QString &comment) {
            if (_comment != comment) {
                _comment = comment;
                setModified(true);
            }
        }

        inline void Annotation::setShape(AnnoShape *shape) {
            if (_shape != shape) {
                _shape = shape;
                setModified(true);
            }
        }

        inline int Annotation::classCount() const {
            return _annoClasses.size();
        }

        inline int Annotation::attributeCount() const {
            return _annoAttributes.size();
        }

        //-----------------------------------------------------------

    }
//end namespace dt
} //end namespace anno

#endif /*ANNOTATION_H_*/


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
