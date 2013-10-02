#ifndef DLGIMEXPORTER_H
#define DLGIMEXPORTER_H

#include <QDialog>
#include "../gen/ui_DlgExporter.h"

namespace anno {
    class ExporterPlugin;
}

class DlgExporter : public QDialog {
        Q_OBJECT

    private:
        Ui::DlgExporterClass ui;

    private:
        void initList();

    private slots:
        void on_cbPluginName_currentIndexChanged(int index);

    public:
        DlgExporter(QWidget *parent = 0);
        ~DlgExporter();

    public:
        int getSelectedExporter() const;
        void setSelectedExporter(int index);
        anno::ExporterPlugin *getSelectedExporterPtr() const;

    public slots:
        virtual void accept();

};

#endif // DLGIMEXPORTER_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
