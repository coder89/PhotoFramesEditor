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

using namespace KIPIPhotoFramesEditor;

class CanvasItemSelectionModel : public QItemSelectionModel
{
    public:

        explicit CanvasItemSelectionModel(QStandardItemModel * model, QObject * parent) :
            QItemSelectionModel(model,parent)
        {
        }

    public Q_SLOTS:

        virtual void select(const QModelIndex & index, QItemSelectionModel::SelectionFlags command)
        {
            if (index.isValid())
            {
                QItemSelection selection(index, index);
                select(selection, command);
            }
        }

        virtual void select(const QItemSelection & selection, QItemSelectionModel::SelectionFlags command)
        {
            if (command == NoUpdate)
                return;
            if (command & QItemSelectionModel::Clear || !(command & command & QItemSelectionModel::Current))
            {
                QItemSelection sel = QItemSelectionModel::selection();
                setSelection(sel, false);
            }
            if (command & QItemSelectionModel::Select)
                setSelection(selection, true);
            if (command & QItemSelectionModel::Deselect)
                setSelection(selection, false);
            if (command & QItemSelectionModel::Rows)
            {}
            if (command &  QItemSelectionModel::Columns)
            {}
            QItemSelectionModel::select(selection,command);
        }

    private:

        void setSelection(const QItemSelection & selection, bool isSelected)
        {
            QModelIndexList list = selection.indexes();
            foreach(QModelIndex index , list)
            {
                QStandardItem * item = (static_cast<const QStandardItemModel*>(index.model()))->itemFromIndex(index);
                AbstractPhoto * g_item = dynamic_cast<AbstractPhoto*>(item);
                if (g_item)
                {
                    g_item->setSelected(isSelected);
                }
            }
        }
};

LayersTree::LayersTree(QWidget *parent) :
    QTreeView(parent)
{
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->header()->setShown(true);
    this->header()->setMovable(false);
    this->header()->setClickable(true);
    this->setDragEnabled(true);
    connect(this->header(), SIGNAL(sectionClicked(int)), this, SLOT(headerSectionClicked(int)));
    //this->header()->setResizeMode(QHeaderView::ResizeToContents);
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
