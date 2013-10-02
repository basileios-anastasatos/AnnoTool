#ifndef COLORFILTERLISTMODELADAPTER_H_
#define COLORFILTERLISTMODELADAPTER_H_

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

namespace anno {
    namespace filter {
        class AnnoFilterManager;
        class ColorFilterEntry;
    }
}

class ColorFilterListModelAdapter : public QAbstractListModel {
        Q_OBJECT

    private:
        anno::filter::AnnoFilterManager *_filterMan;

    private slots:
        void onFM_coloringAdded(::anno::filter::ColorFilterEntry *entry, int index);
        void onFM_coloringRemoved(QString name, int index);
        void onFM_coloringMoved(int oldIndex, int newIndex);

    public:
        ColorFilterListModelAdapter(anno::filter::AnnoFilterManager *filterMan, QObject *parent = 0);

    public:
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const;

        void update();
};

#endif /* COLORFILTERLISTMODELADAPTER_H_ */

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
