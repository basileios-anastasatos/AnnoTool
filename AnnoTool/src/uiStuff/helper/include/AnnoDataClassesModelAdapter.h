#ifndef ANNODATACLASSESMODELADAPTER_H_
#define ANNODATACLASSESMODELADAPTER_H_

#include <QAbstractTableModel>
#include <QVariant>

class AnnoDataClassesModelAdapter : public QAbstractTableModel {
        Q_OBJECT

    public:
        AnnoDataClassesModelAdapter(QObject *parent = 0);
        virtual ~AnnoDataClassesModelAdapter();

    public:
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const;

        void update();
};

#endif /*ANNODATACLASSESMODELADAPTER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
