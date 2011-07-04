#include "Canvas.h"
#include "Scene.h"
#include "LayersModel.h"
#include "LayersSelectionModel.h"
#include "UndoRemoveItem.h"

using namespace KIPIPhotoFramesEditor;

Canvas::Canvas(const QSizeF & dimension, QObject *parent) :
    QObject(parent),
    m_items_group(0),
    m_undo_stack(new QUndoStack(this))
{
    m_scene = new Scene(QRectF(QPointF(0,0), QSizeF(dimension)), this);
    m_model = new LayersModel(this);
    m_selmodel = new LayersSelectionModel(m_model, this);

    // Signals connection
    connect(m_scene, SIGNAL(newItemAdded(AbstractPhoto*)), this, SLOT(addItemToModel(AbstractPhoto*)));
    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect(m_scene, SIGNAL(itemsAboutToBeRemoved(QList<AbstractPhoto*>)), this, SLOT(removeItems(QList<AbstractPhoto*>)));
    connect(m_selmodel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
   // connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT())
}

void Canvas::addImage(const QImage & image)
{
    QSize s = image.size();
    QRect r = image.rect();
    QSize sceneSize = m_scene->sceneRect().size().toSize();
    if (sceneSize.width()<s.width() || sceneSize.height()<s.height())
    {
        s.scale(m_scene->sceneRect().size().toSize()*0.8, Qt::KeepAspectRatio);
        r.setSize(s);
    }
    QPixmap pixmap(s);
    QPainter painter(&pixmap);
    painter.drawImage(r, image);
    QPainterPath asf;
    asf.addRect(QRectF(r));
    PolygonWidget * it = new PolygonWidget(asf,0,m_scene);
    m_scene->addItem(it);
    m_model->prependItem(it);
}

void Canvas::addItemToModel(AbstractPhoto * /*item*/)
{
}

void Canvas::selectionChanged()
{
    QList<AbstractPhoto*> selectedItems = m_scene->selectedItems();
    QModelIndexList newSelected = m_model->itemsToIndexes(selectedItems);
    QModelIndexList oldSelected = m_selmodel->selectedIndexes();
    foreach (QModelIndex index, oldSelected)
    {
        if (!newSelected.contains(index) && index.column() == 0)
            m_selmodel->select(index, QItemSelectionModel::Rows | QItemSelectionModel::Deselect);
    }
    foreach(QModelIndex index, newSelected)
    {
        if (!m_selmodel->isSelected(index) && index.column() == 0)
            m_selmodel->select(index,QItemSelectionModel::Rows | QItemSelectionModel::Select);
    }
}

void Canvas::selectionChanged(const QItemSelection & newSelection, const QItemSelection & oldSelection)
{
    LayersModelItem * temp;
    const QModelIndexList & oldSel = oldSelection.indexes();
    const QModelIndexList & newSel = newSelection.indexes();
    QSet<QModelIndex> deselected = oldSel.toSet().subtract(newSel.toSet());
    foreach (QModelIndex index, deselected)
    {
        if (index.column() != 0)
            continue;
        temp = static_cast<LayersModelItem*>(index.internalPointer());
        if (static_cast<AbstractPhoto*>(temp)->isSelected())
            static_cast<AbstractPhoto*>(temp)->setSelected(false);
    }
    QSet<QModelIndex> selected = newSel.toSet().subtract(oldSel.toSet());
    foreach (QModelIndex index, selected)
    {
        if (index.column() != 0)
            continue;
        temp = static_cast<LayersModelItem*>(index.internalPointer());
        if (!static_cast<AbstractPhoto*>(temp)->isSelected())
            static_cast<AbstractPhoto*>(temp)->setSelected(true);
    }
}

void Canvas::removeItems(const QList<AbstractPhoto*> & items)
{
    foreach (AbstractPhoto * item, items)
    {
        UndoRemoveItem * undo = new UndoRemoveItem(item,m_scene,m_model);
        m_undo_stack->push(undo);
    }
}
