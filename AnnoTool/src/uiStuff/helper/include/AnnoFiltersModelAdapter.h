#ifndef ANNOFILTERSMODELADAPTER_H_
#define ANNOFILTERSMODELADAPTER_H_

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

namespace anno {
    namespace filter {
        class AnnoFilterManager;
        class AnnoFilter;
    }
}

class AnnoFiltersModelAdapter : public QAbstractListModel {
        Q_OBJECT

    private:
        anno::filter::AnnoFilterManager *_filterMan;
        QList<anno::filter::AnnoFilter *> _filterList;

    private slots:
        void onFM_filterAdded(::anno::filter::AnnoFilter *filter);
        void onFM_filterRemoved(QString filterName);

    public:
        AnnoFiltersModelAdapter(anno::filter::AnnoFilterManager *filterMan, QObject *parent = 0);

    public:
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const;

        void update();
};

#endif /* ANNOFILTERSMODELADAPTER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
