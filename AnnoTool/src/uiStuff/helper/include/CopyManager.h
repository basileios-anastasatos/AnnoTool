#ifndef COPYMANAGER_H_
#define COPYMANAGER_H_

#include <QObject>
#include "annoTypesForward.h"

class QClipboard;

class CopyManager : public QObject {
        Q_OBJECT

    private:
        static const QString MIME_ANNOTATION_XML;

    private:
        QClipboard *_clipboard;
        anno::dt::Annotation *_lastAnno;

    private slots:
        void onCb_dataChanged();

    public:
        CopyManager(QObject *parent = 0);

    public:
        bool hasAnnotation() const;
        anno::dt::Annotation *takeAnnotation(bool newId = false,
                                             bool remHierarchy = false, bool remClasses = false, bool remAttributes = false);
        anno::dt::Annotation *getCopyAnnotation(bool newId = false,
                                                bool remHierarchy = false, bool remClasses = false, bool remAttributes = false);
        bool copyAnnotation(const anno::dt::Annotation *anno);

    signals:
        void dataAvailable();
        void lostData();
};

#endif /* COPYMANAGER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
