#include "LayersTree.h"
#include "abstract_photo.h"
#include "LayersTreeDelegate.h"

// Qt
#include <QStandardItemModel>
#include <QDebug>
#include <QHeaderView>
#include <QIcon>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QContextMenuEvent>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::LayersTreeMenu : public QMenu
{
    public:

        LayersTreeMenu(LayersTree * parent) :
            QMenu(parent)
        {
            moveUpItems = this->addAction("Move up");
            connect(moveUpItems, SIGNAL(triggered()), parent, SLOT(moveSelectedRowsUp()));
            moveDownItems = this->addAction("Move down");
            connect(moveDownItems, SIGNAL(triggered()), parent, SLOT(moveSelectedRowsDown()));
            this->addSeparator();
            deleteItems = this->addAction("Delete selected");
            connect(deleteItems, SIGNAL(triggered()), parent, SLOT(removeSelectedRows()));
        }
        void setMoveUpEnabled(bool enabled)
        {
            moveUpItems->setEnabled(enabled);
        }
        void setMoveDownEnabled(bool enabled)
        {
            moveDownItems->setEnabled(enabled);
        }
        void setDeleteEnabled(bool enabled)
        {
            deleteItems->setEnabled(enabled);
        }

    private:

        QAction * moveUpItems;
        QAction * moveDownItems;
        QAction * deleteItems;
};

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
    if (this->selectedIndexes().count())
    {
        QModelIndexList indexList = this->selectedIndexes();
        m_menu->setDeleteEnabled(true);
        m_menu->setMoveDownEnabled(true);
        m_menu->setMoveUpEnabled(true);
        foreach (QModelIndex index, indexList)
        {
            if (index.isValid())
            {
                if (index.row() == 0)
                    m_menu->setMoveUpEnabled(false);
                if (index.row()+1 >= this->model()->rowCount(index.parent()))
                    m_menu->setMoveDownEnabled(false);
            }
            else
                m_menu->setDeleteEnabled(false);
        }
        m_menu->exec(event->globalPos());
    }
}

void LayersTree::removeSelectedRows()
{
    QModelIndexList indexList = this->selectedIndexes();
    QMap<QModelIndex,QList<int> > map;
    foreach (QModelIndex index, indexList)
    {
        if (index.isValid() && index.column() == 0)
        {
            QList<int> list = map.value(index.parent());
            list.append(index.row());
            map.insert(index.parent(),list);
        }
    }
    QList<QModelIndex> parents = map.keys();
    foreach (QModelIndex parent, parents)
    {
        QList<int> list = map.value(parent);
        qSort(list);
        for (int i = list.size()-1; i >= 0; --i)
            this->model()->removeRow(list[i], parent);
    }
}

void LayersTree::moveSelectedRowsUp()
{
    QModelIndexList indexList = this->selectedIndexes();
    qDebug() << showDropIndicator();
}

void LayersTree::moveSelectedRowsDown()
{
}
