#ifndef MOTELITEM_H
#define MOTELITEM_H

#include <QObject>

namespace KIPIPhotoFramesEditor
{
    class LayersModelItem : public QObject
    {
            Q_OBJECT

        public:

            enum DataType
            {
                PadLockIcon, EyeIcon, NameString = 3,
            };

            LayersModelItem(const QList<QVariant> & data, LayersModelItem * parent = 0);
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
            bool setData(const QVariant & data, DataType type);

        public Q_SLOTS:

            virtual void selectStateChanged(bool){};
            virtual void showStateChanged(bool){};
            virtual void lockStateChanged(bool){};

        Q_SIGNALS:

            void selectionChanged(bool);
            void visibilityChanged(bool);
            void blockadeChanged(bool);

        protected:

            LayersModelItem();
            void setData(const QList<QVariant> & data);

        private:

            LayersModelItem * parentItem;
            QList<LayersModelItem*> childItems;
            QList<QVariant> itemData;
    };
}

#endif // MOTELITEM_H
