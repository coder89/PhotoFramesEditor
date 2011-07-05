#ifndef LAYERSTREE_H
#define LAYERSTREE_H

// Qt
#include <QMenu>
#include <QWidget>
#include <QAction>
#include <QTreeView>
#include <QAbstractItemModel>

// Local
#include "LayersModel.h"

class QGraphicsItem;

namespace KIPIPhotoFramesEditor
{
    class LayersTree;
    class LayersTreeMenu;
    class SwitchableIcon;

    class LayersTree : public QTreeView
    {
            Q_OBJECT

        public:

            explicit LayersTree(QWidget * parent = 0);
            virtual void setModel(QAbstractItemModel * model);

        public Q_SLOTS:

        signals:

            void selectedRowsAboutToBeRemoved(const QModelIndexList & selectedIndexes);

        protected:

            void contextMenuEvent(QContextMenuEvent * event);

        protected Q_SLOTS:

            void removeSelectedRows();
            void moveSelectedRowsUp();
            void moveSelectedRowsDown();

        private:

            LayersTreeMenu * m_menu;
    };
}

#endif // LAYERSTREE_H
