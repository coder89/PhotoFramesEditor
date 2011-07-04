#ifndef MODEL_H
#define MODEL_H

// Qt
#include <QObject>
#include <QModelIndex>
#include <QAbstractItemModel>

namespace KIPIPhotoFramesEditor
{
    class LayersModelItem;
    class AbstractPhoto;

    class LayersModel : public QAbstractItemModel
    {

            Q_OBJECT

        public:

            explicit LayersModel(QObject *parent = 0);
            virtual ~LayersModel();

            QModelIndex index( int row, int column, const QModelIndex & parent ) const;
            QModelIndex parent( const QModelIndex & index ) const;
            int rowCount( const QModelIndex & parent = QModelIndex()) const;
            int columnCount( const QModelIndex & parent = QModelIndex()) const;
            QVariant data( const QModelIndex &, int role ) const;
            bool setData(const QModelIndex & index, const QVariant & value, int role);
            Qt::ItemFlags flags(const QModelIndex & index = QModelIndex()) const;
            QVariant headerData(int section, Qt::Orientation orientation, int role) const;
            bool appendItem(AbstractPhoto * item, const QModelIndex & parent = QModelIndex());
            bool prependItem(AbstractPhoto * item, const QModelIndex & parent = QModelIndex());
            bool appendItems(const QList<AbstractPhoto*> & items, const QModelIndex & parent);
            QModelIndexList itemsToIndexes(const QList<AbstractPhoto*> & items) const;
            QModelIndex findIndex(LayersModelItem * item, const QModelIndex & parent = QModelIndex()) const;
            bool insertRows(int row, int count, const QModelIndex  & parent);
            LayersModelItem * getItem(const QModelIndex &index) const;
            bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

            enum ModelRoles
            {
                VisibilityIcon = Qt::UserRole,
                LockIcon,
                GraphicItem
            };

        signals:

            void rowsInserted(const QModelIndex & parent, int first, int last);

        public slots:

            void addNewItem(AbstractPhoto * item, const QModelIndex & parent = QModelIndex());

        private:

            LayersModelItem * root;
    };
}

#endif // MODEL_H
