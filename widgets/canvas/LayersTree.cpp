#include "LayersTree.h"

// Local
#include "abstract_photo.h"
#include "SwitchableIcon.h"

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
    this->header()->setClickable(true);
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setDefaultDropAction(Qt::MoveAction);
    connect(this->header(), SIGNAL(sectionClicked(int)), this, SLOT(headerSectionClicked(int)));
    this->header()->setResizeMode(QHeaderView::ResizeToContents);
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
}

void LayersTree::setModel(QAbstractItemModel * model)
{
   // this->model()->disconnect(this);
    QTreeView::setModel(model);
    this->setupRowsWidgets();

    this->header()->moveSection(1,0);
    this->header()->moveSection(2,1);
    //connect(model,SIGNAL())

    //        QItemSelectionModel * todelete = selectionModel();
    //        setSelectionModel(new CanvasItemSelectionModel(model,this));
    //        delete todelete;
    model->setHeaderData(1, Qt::Vertical, "Long Test");
}

void LayersTree::setupRowsWidgets(const QModelIndex & parent)
{
    for (int i = 0; i < model()->rowCount(); ++i)
    {
        setRowWidgets(parent, i);
        QModelIndex child = model()->index(i,0,parent);
        if (child.isValid())
            setupRowsWidgets(model()->index(i,0,parent));
    }
}

void LayersTree::rowsInserted(const QModelIndex & parent, int start, int end)
{
    qDebug () << "rowsInserted" << start << end;
    for (; start <= end; ++start)
        setRowWidgets(parent, start);
    QTreeView::rowsInserted(parent,start,end);
}

void LayersTree::setRowWidgets(const QModelIndex & parent, int row)
{
    //QModelIndex child1 = model()->index(row, 1, parent);
    //QVariant data1 = model()->data(child1, LayersModel::VisibilityIcon);
    //SwitchableIcon * l1 = new SwitchableIcon(data1.value<QIcon>(), static_cast<LayersModelItem*>(child1.internalPointer()));
    //connect(l1, SIGNAL(clicked(SwitchableIcon*)), this, SLOT(hide(SwitchableIcon*)));
    //this->setIndexWidget(child1, l1);

    //QModelIndex child2 = model()->index(row, 2, parent);
    //QVariant data2 = model()->data(child2, LayersModel::LockIcon);
    //SwitchableIcon * l2 = new SwitchableIcon(data2.value<QIcon>(), static_cast<LayersModelItem*>(child2.internalPointer()));
    //l2->toggle();
    //connect(l2, SIGNAL(clicked(SwitchableIcon*)), this, SLOT(lock(SwitchableIcon*)));
    //this->setIndexWidget(model()->index(row, 2, parent), l2);
}

void LayersTree::hide(SwitchableIcon * icon)
{
    AbstractPhoto * item = static_cast<AbstractPhoto*>(icon->item());
    item->setVisible(!item->isVisible());
}

void LayersTree::lock(SwitchableIcon * icon)
{
    AbstractPhoto * item = static_cast<AbstractPhoto*>(icon->item());
    item->setFlag(QGraphicsItem::ItemIsMovable, (item->flags() & QGraphicsItem::ItemIsMovable) ^ QGraphicsItem::ItemIsMovable);
}

void LayersTree::headerSectionClicked(int section)
{
    qDebug() << this->verticalOffset() << this->horizontalOffset();
    // TODO : Selection of hide/lock icons from header
    qDebug() << section;
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
                if (index.column() == 0)
                {
                    if (index.row() == 0)
                        m_menu->setMoveUpEnabled(false);
                    if (index.row()+1 >= this->model()->rowCount(index.parent()))
                        m_menu->setMoveDownEnabled(false);
                }
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
