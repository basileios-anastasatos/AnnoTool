#ifndef DLGIMPORTER_H
#define DLGIMPORTER_H

#include <QDialog>
#include "../gen/ui_DlgImporter.h"

namespace anno {
    class ImporterPlugin;
}

class DlgImporter : public QDialog {
        Q_OBJECT

    private:
        Ui::DlgImporterClass ui;

    private:
        void initList();

    private slots:
        void on_cbPluginName_currentIndexChanged(int index);

    public:
        DlgImporter(QWidget *parent = 0);
        ~DlgImporter();

    public:
        int getSelectedImporter() const;
        void setSelectedImporter(int index);
        anno::ImporterPlugin *getSelectedImporterPtr() const;

    public slots:
        virtual void accept();

};

#endif // DLGIMPORTER_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
