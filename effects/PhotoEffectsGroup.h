#ifndef ABSTRACTPHOTOEFFECTSGROUP_H
#define ABSTRACTPHOTOEFFECTSGROUP_H

#include <QAbstractItemModel>
#include <QPixmap>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;
    class PhotoEffectsLoader;
    class AbstractPhotoEffectInterface;

    class PhotoEffectsGroup : public QAbstractItemModel
    {
            Q_OBJECT

            AbstractPhoto * m_photo;
            //QList<AbstractPhotoEffect*> m_effects_list;
            QList<AbstractPhotoEffectInterface*> m_effects_list;

        public:

            explicit PhotoEffectsGroup(AbstractPhoto * photo, QObject * parent = 0);
            AbstractPhoto * photo() const;
            //AbstractPhotoEffect * getItem(const QModelIndex & index = QModelIndex()) const;
            AbstractPhotoEffectInterface * getItem(const QModelIndex & index = QModelIndex()) const;

            // Reimplemented QAbstractItemModel methods
            virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
            virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
            virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
            virtual QModelIndex parent( const QModelIndex & index ) const;
            virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

        signals:

            void effectsChanged();

        public slots:

//            void push_back(AbstractPhotoEffect * effect);
//            void push_front(AbstractPhotoEffect * effect);
//            void emitEffectsChanged(AbstractPhotoEffect * effect);
//            void emitEffectsChanged(AbstractPhotoEffectInterface * effect);
//            QPixmap apply(const QPixmap & pixmap);

            void push_back(AbstractPhotoEffectInterface * effect);
            void push_front(AbstractPhotoEffectInterface * effect);
            void emitEffectsChanged(AbstractPhotoEffectInterface * effect);
            QPixmap apply(const QPixmap & pixmap);

    };
}

#endif // ABSTRACTPHOTOEFFECTSGROUP_H
