#ifndef MOTELITEM_H
#define MOTELITEM_H

#include <QObject>
#include <QDebug>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;

    class LayersModelItem : public QObject
    {
            Q_OBJECT

            static const int COLUMN_COUNT = 4;

        public:

            enum
            {
                EyeIcon = 1, PadLockIcon, NameString = COLUMN_COUNT-1,
            };

            LayersModelItem(AbstractPhoto * item, LayersModelItem * parent = 0);
            LayersModelItem();
            virtual ~LayersModelItem();
            void removeChild(LayersModelItem * child);
            LayersModelItem * parent() const;
            void setParent(LayersModelItem * parent);
            int row() const;
            int columnCount() const;
            int childCount() const;
            LayersModelItem * child(int row) const;
            QVariant data(int column) const;
            QList<QVariant> data() const;
            bool insertChildren(int position, LayersModelItem * item);
            bool removeChildren(int position, int count);
            bool moveChildren(int sourcePosition, int count, LayersModelItem * destParent, int destPosition);
            bool setData(const QVariant & data, int type);
            void setPhoto(AbstractPhoto * photo);
            AbstractPhoto * photo() const;

        public Q_SLOTS:

            virtual void selectStateChanged(bool){};
            virtual void showStateChanged(bool){};
            virtual void lockStateChanged(bool){};

        Q_SIGNALS:

            void selectionChanged(bool);
            void visibilityChanged(bool);
            void blockadeChanged(bool);

        protected:

            void setData(const QList<QVariant> & data);

        private:

            void refreshZValues();

            LayersModelItem * parentItem;
            QList<LayersModelItem*> childItems;
            AbstractPhoto * itemPhoto;
    };
}

#endif // MOTELITEM_H
