#include "LayersTree.h"
#include "abstract_photo.h"
#include "LayersTreeDelegate.h"
#include "LayersTreeMenu.h"

// Qt
#include <QStandardItemModel>
#include <QDebug>
#include <QHeaderView>
#include <QIcon>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QContextMenuEvent>

using namespace KIPIPhotoFramesEditor;

LayersTree::LayersTree(QWidget * parent) :
    QTreeView(parent),
    m_menu(new LayersTreeMenu(this))
{
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->header()->setShown(true);
    this->header()->setMovable(false);
    this->header()->setClickable(false);
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setDefaultDropAction(Qt::MoveAction);
    this->header()->setResizeMode(QHeaderView::ResizeToContents);
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAnimated(true);
}

void LayersTree::setModel(QAbstractItemModel * model)
{
    QTreeView::setModel(model);

    QAbstractItemDelegate * delegate1 = this->itemDelegateForColumn(1);
    QAbstractItemDelegate * delegate2 = this->itemDelegateForColumn(2);
    LayersTreeDelegate * layersDelegate1 = dynamic_cast<LayersTreeDelegate*>(delegate1);
    if (!layersDelegate1)
    {
        layersDelegate1 = new LayersTreeDelegate(this);
        this->setItemDelegateForColumn(1,layersDelegate1);
        connect(this, SIGNAL(clicked(QModelIndex)), layersDelegate1, SLOT(itemClicked(QModelIndex)));
        connect(layersDelegate1, SIGNAL(itemRepaintNeeded(QModelIndex)), this, SLOT(update(QModelIndex)));
    }
    LayersTreeDelegate * layersDelegate2 = dynamic_cast<LayersTreeDelegate*>(delegate2);
    if (!layersDelegate2)
        this->setItemDelegateForColumn(2,layersDelegate1);

    this->header()->moveSection(1,0);
    this->header()->moveSection(2,1);
}

void LayersTree::contextMenuEvent(QContextMenuEvent * event)
{
    QModelIndexList indexList = this->selectedIndexes();
    if (indexList.count())
    {
        m_menu->setDeleteEnabled(true);
        m_menu->setMoveDownEnabled(false);
        m_menu->setMoveUpEnabled(false);

        // Disables unsupported move operations
        QModelIndexList::iterator it = indexList.begin();
        QModelIndex startIndex = *it;
        unsigned int minRow;
        unsigned int maxRow;
        unsigned int sumRows;
        if (!startIndex.isValid())
            goto end_moving_menus;      // It's not so bad as many people think ;) Here 'goto' simplyfies code!
        minRow = it->row();
        maxRow = it->row();
        sumRows = it->row();
        for (++it; it != indexList.end(); ++it)
        {
            if (!it->isValid())
            {
                event->setAccepted(false);
                return;
            }
            else if (startIndex.parent() != it->parent())
                goto end_moving_menus;  // It's not so bad as many people think ;) Here 'goto' simplyfies code!
            if (it->row() < minRow)
                minRow = it->row();
            if (it->row() > maxRow)
                maxRow = it->row();
            sumRows += it->row();
        }

        if ((((minRow+maxRow)*(maxRow-minRow+1))/2.0) == sumRows)
        {
            if (minRow > 0)
                m_menu->setMoveUpEnabled(true);
            if (maxRow+1 < this->model()->rowCount(indexList.first().parent()))
                m_menu->setMoveDownEnabled(true);
        }

        end_moving_menus:

        // Shows menu
        m_menu->exec(event->globalPos());
    }
}

void LayersTree::removeSelectedRows()
{
    emit selectedRowsAboutToBeRemoved();
}

void LayersTree::moveSelectedRowsUp()
{
    emit selectedRowsAboutToBeMovedUp();
}

void LayersTree::moveSelectedRowsDown()
{
    emit selectedRowsAboutToBeMovedDown();
}
