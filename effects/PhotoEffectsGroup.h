#ifndef ABSTRACTPHOTOEFFECTSGROUP_H
#define ABSTRACTPHOTOEFFECTSGROUP_H

#include <QAbstractItemModel>
#include <QPixmap>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;
    class PhotoEffectsLoader;
    class MoveItemsUndoCommand;
    class RemoveItemsUndoCommand;
    class AbstractPhotoEffectInterface;

    class PhotoEffectsGroup : public QAbstractItemModel
    {
            Q_OBJECT

            AbstractPhoto * m_photo;
            QList<AbstractPhotoEffectInterface*> m_effects_list;

        public:

            explicit PhotoEffectsGroup(AbstractPhoto * photo, QObject * parent = 0);
            AbstractPhoto * photo() const;
            AbstractPhotoEffectInterface * getItem(const QModelIndex & index = QModelIndex()) const;
            bool moveRows(int sourcePosition, int sourceCount, int destPosition);

            // Reimplemented QAbstractItemModel methods
            virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
            virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
            virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
            virtual QModelIndex parent( const QModelIndex & index ) const;
            virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
            virtual bool removeRows(int row, int count, const QModelIndex & parent);

        signals:

            void effectsChanged();

        public slots:

            void push_back(AbstractPhotoEffectInterface * effect);
            void push_front(AbstractPhotoEffectInterface * effect);
            void emitEffectsChanged(AbstractPhotoEffectInterface * effect = 0);
            QPixmap apply(const QPixmap & pixmap);

        private:

            void moveRowsInModel(int sourcePosition, int sourceCount, int destPosition);
            QList<AbstractPhotoEffectInterface*> removeRowsInModel(int startingPosition, int count);
            void insertRowsInModel(const QList<AbstractPhotoEffectInterface*> & itemList, int startingPosition);

        friend class MoveItemsUndoCommand;
        friend class RemoveItemsUndoCommand;
    };
}

#endif // ABSTRACTPHOTOEFFECTSGROUP_H
