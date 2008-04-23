#include "include/DlgLoaderDetails.h"
#include "GlobalImageLoader.h"

using ::anno::GlobalImageLoader;

DlgLoaderDetails::DlgLoaderDetails(QWidget *parent) :
    QDialog(parent) {
    ui.setupUi(this);
    initLoaderData();
}

DlgLoaderDetails::~DlgLoaderDetails() {

}

void DlgLoaderDetails::initLoaderData() {
    GlobalImageLoader *il = GlobalImageLoader::instance();
    int count = il->pluginCount();
    for (int i = 0; i < count; ++i) {
        anno::ImageLoaderPlugin *p = il->getPlugin(i);
        ui.cbLoader->addItem(p->name());
    }
}

QString DlgLoaderDetails::listToString(const QString &sep,
                                       const QList<QString> &lst) {
    QString ret;
    if (!lst.isEmpty()) {
        QListIterator<QString> i(lst);
        while (i.hasNext()) {
            ret.append(i.next());
            ret.append(sep);
        }
        ret.remove(ret.size() - sep.size(), sep.size());
    }
    return ret;
}

void DlgLoaderDetails::on_cbLoader_currentIndexChanged(int index) {
    anno::ImageLoaderPlugin *p = GlobalImageLoader::instance()->getPlugin(index);
    if (p != NULL) {
        ui.editAut->setText(p->author());
        QList<QString> fl = p->fileTypes();
        if (fl.isEmpty()) {
            ui.editExt->setText(listToString(" ", fl));
        } else {
            ui.editExt->setText("<< all extensions >>");
        }
        ui.txtDesc->setText(p->description());
        ui.txtImages->setText(listToString("\n", p->imageTypes()));
    }
}


// vim:ts=4:sts=4:sw=4:tw=80:expandtab
