#include "include/CopyManager.h"
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QByteArray>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QUuid>
#include "importGlobals.h"
#include "Annotation.h"
#include "XmlHelper.h"
#include "AllAnnoExceptions.h"

using anno::helper::XmlHelper;

const QString CopyManager::MIME_ANNOTATION_XML("application/annotool.anno-xml");

CopyManager::CopyManager(QObject *parent) : QObject(parent), _lastAnno(NULL) {
    _clipboard = QApplication::clipboard();

    connect(_clipboard, SIGNAL(dataChanged()), this, SLOT(onCb_dataChanged()));
}

void CopyManager::onCb_dataChanged() {
    const QMimeData *data = _clipboard->mimeData();
    if(data != NULL && data->hasFormat(MIME_ANNOTATION_XML)) {
        QByteArray encodedData = data->data(MIME_ANNOTATION_XML);
        QString xmlData = QString::fromUtf8(encodedData);
        QXmlStreamReader reader(xmlData);
        reader.setNamespaceProcessing(true);

        if(!XmlHelper::skipToStartElement("annotation", reader)) {
            GlobalLogger::instance()->logError("Found an annotation on the clipboard but could not find the starting tag.");
            return;
        }

        anno::dt::Annotation *curAnno = NULL;
        try {
            curAnno = anno::dt::Annotation::fromXml(reader);
        } catch(anno::exc::AnnoException *e) {
            GlobalLogger::instance()->logError(QString("Found an annotation on the clipboard but could not it. Exception was: %1").arg(e->getTrace()));
            delete e;
            return;
        }

        if(_lastAnno != NULL) {
            delete _lastAnno;
        }
        _lastAnno = curAnno;

        emit dataAvailable();
    }
}

bool CopyManager::hasAnnotation() const {
    return (_lastAnno != NULL);
}

anno::dt::Annotation *CopyManager::takeAnnotation(bool newId, bool remHierarchy, bool remClasses, bool remAttributes) {
    if(_lastAnno == NULL) {
        return NULL;
    }

    anno::dt::Annotation *anno = _lastAnno;

    if(newId) {
        anno->setAnnoId(QUuid::createUuid());
    }
    if(remHierarchy) {
        anno->setAnnoParent(QUuid());
        anno->removeAnnoChildAll();
    }
    if(remClasses) {
        anno->removeClassAll();
    }
    if(remAttributes) {
        anno->removeAttributeAll();
    }

    _lastAnno = NULL;
    emit lostData();
    return anno;
}

anno::dt::Annotation *CopyManager::getCopyAnnotation(bool newId, bool remHierarchy, bool remClasses, bool remAttributes) {
    if(_lastAnno == NULL) {
        return NULL;
    }

    anno::dt::Annotation *anno = new anno::dt::Annotation(_lastAnno);

    if(newId) {
        anno->setAnnoId(QUuid::createUuid());
    }
    if(remHierarchy) {
        anno->setAnnoParent(QUuid());
        anno->removeAnnoChildAll();
    }
    if(remClasses) {
        anno->removeClassAll();
    }
    if(remAttributes) {
        anno->removeAttributeAll();
    }

    return anno;
}

bool CopyManager::copyAnnotation(const anno::dt::Annotation *anno) {
    QString xmlData;
    QXmlStreamWriter writer(&xmlData);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    try {
        anno->toXml(writer);
    } catch(anno::exc::AnnoException *e) {
        GlobalLogger::instance()->logError(QString("Attempt to serialize annotation for clipboard failed. Exception was: %1").arg(e->getTrace()));
        delete e;
        return false;
    }

    writer.writeEndDocument();

    //GlobalLogger::instance()->logInfo(xmlData);

    QMimeData *mimeData = new QMimeData();
    mimeData->setData(MIME_ANNOTATION_XML, xmlData.toUtf8());
    _clipboard->setMimeData(mimeData);

    return true;
}




// vim:ts=4:sts=4:sw=4:tw=80:expandtab
