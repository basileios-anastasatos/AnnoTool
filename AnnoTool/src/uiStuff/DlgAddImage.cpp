#include "include/DlgAddImage.h"
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include "GlobalProjectManager.h"

using ::anno::GlobalProjectManager;

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
    QStringList files = QFileDialog::getOpenFileNames(this,
                        tr("Select Image Files"));
    if (!files.isEmpty()) {
        QStringList lst = files;
        QListIterator<QString> i(lst);
        while (i.hasNext()) {
            QString imgPath = i.next();
            if (!_imageList.contains(imgPath)) {
                QFileInfo info(imgPath);
                QListWidgetItem *item = new QListWidgetItem(ui.lstImages);
                item->setText(info.fileName());
                item->setToolTip(imgPath);
                ui.lstImages->addItem(item);
                _imageList.append(imgPath);
            }
        }
        ui.lbCount->setText(_strCount.arg(_imageList.size()));
    }
}

QList<QString> DlgAddImage::images() const {
    return _imageList;
}

bool DlgAddImage::saveImagesRel() const {
    return ui.ckSaveRel->isChecked();
}

QString DlgAddImage::annoSavePath() const {
    return ui.cbPath->lineEdit()->text();
}

void DlgAddImage::accept() {
    QFileInfo info(annoSavePath());
    if (info.isRelative()) {
        info = GlobalProjectManager::instance()->relToAbs(info);
    }

    if (!info.exists() || !info.isDir())
        QMessageBox::critical(this, "Invalid Input Data",
                              "Please select a valid folder as Annotation File Path!");
    else {
        QDialog::accept();
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
