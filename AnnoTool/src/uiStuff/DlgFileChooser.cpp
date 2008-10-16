#include "include/DlgFileChooser.h"
#include <QFileDialog>
#include <QMessageBox>

DlgFileChooser::DlgFileChooser(QWidget *parent, bool selFolder) :
    QDialog(parent) {
    ui.setupUi(this);
    _selFolder = selFolder;
}

DlgFileChooser::~DlgFileChooser() {
}

void DlgFileChooser::on_btSelect_clicked() {
    QString curPath = ui.editPath->text();
    if (_selFolder) {
        QString path = QFileDialog::getExistingDirectory(this, tr("Select a Folder"), curPath);
        ui.editPath->setText(path);
    } else {
        QString path;
        if (_filters.isEmpty()) {
            path = QFileDialog::getOpenFileName(this, tr("Select a File"), curPath);
        } else {
            path = QFileDialog::getOpenFileName(this, tr("Select a File"), curPath, _filters);
        }

        ui.editPath->setText(path);
    }
}

void DlgFileChooser::setSelectFilter(const QString &filters) {
    _filters = filters;
}

void DlgFileChooser::setRelativePath(bool val) {
    ui.cbRelative->setChecked(val);
}

QString DlgFileChooser::selectedPath() const {
    return ui.editPath->text();
}

bool DlgFileChooser::selectedDir() const {
    QFileInfo info(ui.editPath->text());
    return info.isDir();
}

bool DlgFileChooser::selectedFile() const {
    QFileInfo info(ui.editPath->text());
    return info.isFile();
}

bool DlgFileChooser::useRelativePath() const {
    return ui.cbRelative->isChecked();
}

void DlgFileChooser::accept() {
    if (_selFolder && !selectedDir()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(this,
                                          "Are you sure?",
                                          "You did not select a valid directory!\nDo you want to proceed?",
                                          QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok) {
            QDialog::accept();
        }
    } else if (!_selFolder && !selectedFile()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(this,
                                          "Are you sure?",
                                          "You did not select a valid file!\nDo you want to proceed?",
                                          QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok) {
            QDialog::accept();
        }
    } else {
        QDialog::accept();
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
