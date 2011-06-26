#ifndef LAYERSTREE_H
#define LAYERSTREE_H

// Qt
#include <QWidget>
#include <QTreeView>
#include <QAbstractItemModel>

// Local
#include "LayersModel.h"

class QGraphicsItem;

namespace KIPIPhotoFramesEditor
{
    class CanvasItemSelectionModel;
    class SwitchableIcon;

    class LayersTree : public QTreeView
    {
            Q_OBJECT

        public:

            explicit LayersTree(QWidget * parent = 0);
            virtual void setModel(QAbstractItemModel * model);

        public Q_SLOTS:

            void hide(SwitchableIcon * icon);
            void lock(SwitchableIcon * icon);
            void headerSectionClicked(int section);

        signals:

        protected Q_SLOTS:

            void rowsInserted(const QModelIndex & parent, int start, int end);
            void setRowWidgets(const QModelIndex & parent, int row);
            void setupRowsWidgets(const QModelIndex & parent = QModelIndex());

        private:

            friend class CanvasItemSelectionModel;
    };
}

#endif // LAYERSTREE_H
