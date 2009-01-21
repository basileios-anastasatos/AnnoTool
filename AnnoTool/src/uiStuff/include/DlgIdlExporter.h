#ifndef DLGIDLEXPORTER_H
#define DLGIDLEXPORTER_H

#include <QtGui/QDialog>
#include "ui_DlgIdlExporter.h"
#include "annoFiltersForward.h"

class DlgIdlExporter: public QDialog {
        Q_OBJECT

    private:
        Ui::DlgIdlExporterClass ui;
        anno::filter::AnnoFilter *_selFilter;

    private slots:
        void on_cbFilters_currentIndexChanged(int idx);
        void on_btFilterEdit_clicked();

    private:
        void initFilters();

    public:
        DlgIdlExporter(QWidget *parent = 0);
        ~DlgIdlExporter();

    public:
        bool expRectangles() const;
        bool expEllipses() const;
        bool expPolygons() const;
        bool expSinglePoints() const;
        bool expAnything() const;
        bool expAll() const;
        bool pathsRelative() const;
        bool pathsAbsolute() const;
        bool hasFilter() const;
        anno::filter::AnnoFilter *selectedFilter() const;
};

#endif // DLGIDLEXPORTER_H

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
