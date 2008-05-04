#include "include/DlgAddImage.h"
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QInputDialog>
#include <QRegExp>
#include "importGlobals.h"

DlgAddImage::DlgAddImage(QWidget *parent) :
    QDialog(parent), _strCount("%1 images selected") {
    ui.setupUi(this);
    ui.lbCount->setText(_strCount.arg(0));
    initAnnoPath();
}

DlgAddImage::~DlgAddImage() {

}

void DlgAddImage::initAnnoPath() {
    GlobalProjectManager *pm = GlobalProjectManager::instance();
    if (pm->isValid()) {
        QList<QFileInfo> *lst = pm->project()->searchPath();
        if (!lst->isEmpty()) {
            QListIterator<QFileInfo> i(*lst);
            while (i.hasNext()) {
                QFileInfo cur = i.next();
                ui.cbPath->addItem(cur.filePath());
            }
        }
    }
}

void DlgAddImage::on_btSelPath_clicked() {
    QString path = QFileDialog::getExistingDirectory(this,
                   tr("Select Annotation Save Folder"));
    ui.cbPath->lineEdit()->setText(path);
}

void DlgAddImage::on_btAddImages_clicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Image Files"));
    if (!files.isEmpty()) {
        QStringList lst = files;
        QListIterator<QString> i(lst);
        while (i.hasNext()) {
            QString imgPath = i.next();
            QPair<QString, int> tmp(imgPath, NOFRAME);
            if (!_imageList.contains(tmp)) {
                QFileInfo info(imgPath);
                QListWidgetItem *item = new QListWidgetItem(ui.lstImages);
                item->setText(info.fileName());
                item->setToolTip(imgPath);
                ui.lstImages->addItem(item);
                _imageList.append(tmp);
            }
        }
        qSort(_imageList.begin(), _imageList.end());
        ui.lstImages->sortItems();
        ui.lbCount->setText(_strCount.arg(_imageList.size()));
    }
}

void DlgAddImage::on_btAddVideo_clicked() {
    QString vidPath = QFileDialog::getOpenFileName(this, tr("Select Video File"));
    if (!vidPath.isEmpty()) {
        QString range = QInputDialog::getText(this, "Select Video frames", "Frames to use");
        GlobalLogger::instance()->logDebug(QString("Selected video [%1] with frame range [%2].").arg(vidPath).arg(range));
        if (!range.isEmpty()) {
            QRegExp ex("([0-9;-])+");
            if (!ex.exactMatch(range)) {
                QMessageBox::critical(this, "Invalid Frame Range",
                                      "Given frame range string contains invalid characters.");
                return;
            }
            QStringList lst = range.split(";", QString::SkipEmptyParts);
            QList< QPair<QString, int> > tmpLst;
            QListIterator<QString> i(lst);
            QPair<QString, int> tmpPair(vidPath, NOFRAME);
            while (i.hasNext()) {
                QString cur = i.next();
                if (cur.contains("-")) {
                    QStringList rlst = cur.split("-", QString::SkipEmptyParts);
                    if (rlst.size() != 2) {
                        GlobalLogger::instance()->logError(QString("Unable to split [%1]").arg(cur));
                        QMessageBox::critical(this, "Invalid Frame Range",
                                              "Cannot extract frame range.");
                        return;
                    }
                    bool ok = false;
                    int start = rlst[0].toInt(&ok, 10);
                    int end = rlst[1].toInt(&ok, 10);
                    if (!ok) {
                        QMessageBox::critical(this, "Invalid Frame Range",
                                              "Cannot extract frame number.");
                        return;
                    }
                    if (start > end) {
                        int tmp = start;
                        start = end;
                        end = tmp;
                    }
                    for (int i = start; i <= end; ++i) {
                        tmpPair.second = i;
                        if (!tmpLst.contains(tmpPair)) {
                            tmpLst.append(tmpPair);
                        }
                    }
                } else {
                    bool ok = false;
                    int frame = cur.toInt(&ok, 10);
                    if (!ok) {
                        QMessageBox::critical(this, "Invalid Frame Range",
                                              "Cannot extract frame number.");
                        return;
                    }

                    tmpPair.second = frame;
                    if (!tmpLst.contains(tmpPair)) {
                        tmpLst.append(tmpPair);
                    }
                }
            }

            QListIterator< QPair<QString, int> > it(tmpLst);
            while (it.hasNext()) {
                QPair<QString, int> cur = it.next();
                if (!_imageList.contains(cur)) {
                    QFileInfo info(cur.first);
                    QListWidgetItem *item = new QListWidgetItem(ui.lstImages);
                    item->setText(info.fileName() + QString("#%1").arg(cur.second));
                    item->setToolTip(cur.first + QString("#%1").arg(cur.second));
                    ui.lstImages->addItem(item);
                    _imageList.append(cur);
                }
            }
            qSort(_imageList.begin(), _imageList.end());
            ui.lstImages->sortItems();
            ui.lbCount->setText(_strCount.arg(_imageList.size()));
        }
    }
}

QList< QPair<QString, int> > DlgAddImage::images() const {
    return _imageList;
}

bool DlgAddImage::saveImagesRel() const {
    return ui.ckSaveRel->isChecked();
}

QString DlgAddImage::annoSavePath() const {
    return ui.cbPath->lineEdit()->text();
}

void DlgAddImage::accept() {
    QString path = annoSavePath();
    if (path.isEmpty() && !_imageList.isEmpty()) {
        QMessageBox::critical(this, "Invalid Input Data",
                              "You must select a path where to store\nthe new annotation files.");
        return;
    }

    QFileInfo info(path);
    if (info.isRelative()) {
        info = GlobalProjectManager::instance()->relToAbs(info);
    }

    if (!_imageList.isEmpty() && (!info.exists() || !info.isDir()))
        QMessageBox::critical(this, "Invalid Input Data",
                              "Please select a valid folder as Annotation File Path!");
    else {
        QDialog::accept();
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
