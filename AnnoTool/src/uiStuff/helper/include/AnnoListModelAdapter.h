#ifndef ANNOLISTMODELADAPTER_H_
#define ANNOLISTMODELADAPTER_H_

#include <QAbstractItemModel>
#include <QVariant>
#include <QMimeData>

namespace anno {
    namespace dt {
        class AnnoFileData;
        class Annotation;
    }
}

class AnnoListModelAdapter : public QAbstractItemModel {
        Q_OBJECT

    private:
        //const int rootId = -42;

    private:
        int parentScopeRow(anno::dt::AnnoFileData *curFile, const anno::dt::Annotation *anno) const;

    public:
        AnnoListModelAdapter(QObject *parent = 0);
        virtual ~AnnoListModelAdapter();

    public:
        virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const;

        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual Qt::DropActions supportedDropActions() const;
        virtual QStringList mimeTypes() const;
        virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
        virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

        virtual QModelIndex index(int row, int column,
                                  const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &index) const;

        void update();

    signals:
        void dropEventFinished();
};

#endif /*ANNOLISTMODELADAPTER_H_*/

// vim:ts=4:sts=4:sw=4:tw=80:expandtab
