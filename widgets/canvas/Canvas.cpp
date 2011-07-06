#include "Canvas.h"
#include "Scene.h"
#include "LayersModel.h"
#include "LayersModelItem.h"
#include "LayersSelectionModel.h"
#include "UndoRemoveItem.h"

#include <kapplication.h>
#include <kmessagebox.h>
#include <klocalizedstring.h>

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
    connect(m_scene, SIGNAL(itemAboutToBeRemoved(AbstractPhoto*)), this, SLOT(removeItem(AbstractPhoto*)));
    connect(m_scene, SIGNAL(itemsAboutToBeRemoved(QList<AbstractPhoto*>)), this, SLOT(removeItems(QList<AbstractPhoto*>)));
    connect(m_selmodel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
}

void Canvas::addImage(const QImage & image)
{
    // Read image and create pixmap
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

    /// TODO: REMOVE AFTER TESTS!
    QPainterPath asf;
    asf.addRect(QRectF(r));

    // Create & setup item
    PolygonWidget * it = new PolygonWidget(asf,0,m_scene);  /// TODO : Change to image item - NOT POLYGON!!!!
    it->setName(image.text("File").append(QString::number(m_model->rowCount())));
    it->setZValue(m_model->rowCount());

    // Add item to scene & model
    m_scene->addItem(it);
    m_model->prependItem(it);

    if (m_selmodel->hasSelection())
    {} /// TODO: add above selected
}

void Canvas::addItemToModel(AbstractPhoto * /*item*/)
{
}

/** ##########################################################################################################################
 * Creates item move up command and push it to the undo stack
 #############################################################################################################################*/
void Canvas::moveUpCommand()
{}

/** ##########################################################################################################################
 * Creates item move down command and push it to the undo stack
 #############################################################################################################################*/
void Canvas::moveDownCommand()
{}

/** ##########################################################################################################################
 * Move selected items up on scene & model. (Called by layers tree)
 #############################################################################################################################*/
void Canvas::moveSelectedRowsUp(const QModelIndexList & selectedIndexes)
{}

/** ##########################################################################################################################
 * Move selected items down on scene & model. (Called by layers tree)
 #############################################################################################################################*/
void Canvas::moveSelectedRowsDown(const QModelIndexList & selectedIndexes)
{}

/** ##########################################################################################################################
 * Creates item remove command and push it to the undo stack
 #############################################################################################################################*/
void Canvas::removeComand(AbstractPhoto * item)
{
    UndoRemoveItem * undo = new UndoRemoveItem(item,m_scene,m_model);
    m_undo_stack->push(undo);
}

/** ##########################################################################################################################
 * Remove from model ONE item removed from scene
 #############################################################################################################################*/
void Canvas::removeItem(AbstractPhoto * item)
{
    if (item && askAboutRemoving(1))
        this->removeComand(item);
}

/** ##########################################################################################################################
 * Remove from model items removed from scene
 #############################################################################################################################*/
void Canvas::removeItems(const QList<AbstractPhoto*> & items)
{
    if (askAboutRemoving(items.count()))
    {
        if (items.count() > 1)
            beginRowsRemoving();
        foreach (AbstractPhoto * item, items)
            this->removeComand(item);
        if (items.count() > 1)
            endRowsRemoving();
    }
}

/** ##########################################################################################################################
 * Remove from scene items removed from layert tree.
 #############################################################################################################################*/
void Canvas::removeSelectedRows(const QModelIndexList & selectedIndexes)
{
    if (askAboutRemoving(selectedIndexes.count()))
    {
        if (selectedIndexes.count() > 1)
            beginRowsRemoving();
        foreach (QModelIndex index, selectedIndexes)
        {
            if (index.column() != LayersModelItem::NameString)
                continue;
            AbstractPhoto * photo = static_cast<LayersModelItem*>(index.internalPointer())->photo();
            removeComand(photo);
        }
        if (selectedIndexes.count() > 1)
            endRowsRemoving();
    }
}

/** ##########################################################################################################################
 * Ask about removing @arg count of items
 #############################################################################################################################*/
bool Canvas::askAboutRemoving(int count)
{
    if (count)
    {
        int result = KMessageBox::questionYesNo(KApplication::activeWindow(), i18n("Are you sure you want to delete ").append(count).append(i18n(" selected items?")), i18n("Items deleting"));
        if (result == KMessageBox::Yes)
            return true;
    }
    return false;
}

/** ###########################################################################################################################
 * Synchronize scene's selection with model's selection
 #############################################################################################################################*/
void Canvas::selectionChanged()
{
    QList<AbstractPhoto*> selectedItems = m_scene->selectedItems();
    QModelIndexList newSelected = m_model->itemsToIndexes(selectedItems);
    QModelIndexList oldSelected = m_selmodel->selectedIndexes();
    foreach (QModelIndex index, oldSelected)
    {
        if (!newSelected.contains(index) && index.column() == LayersModelItem::NameString)
            m_selmodel->select(index, QItemSelectionModel::Deselect);
    }
    foreach(QModelIndex index, newSelected)
    {
        if (!m_selmodel->isSelected(index) && index.column() == LayersModelItem::NameString)
            m_selmodel->select(index, QItemSelectionModel::Select);
    }
}

/** ###########################################################################################################################
 * Synchronize model's selection with scene's selection
 #############################################################################################################################*/
void Canvas::selectionChanged(const QItemSelection & newSelection, const QItemSelection & oldSelection)
{
    LayersModelItem * temp;
    const QModelIndexList & oldSel = oldSelection.indexes();
    const QModelIndexList & newSel = newSelection.indexes();
    QSet<QModelIndex> deselected = oldSel.toSet().subtract(newSel.toSet());
    foreach (QModelIndex index, deselected)
    {
        if (index.column() != LayersModelItem::NameString)
            continue;
        temp = static_cast<LayersModelItem*>(index.internalPointer());
        if (temp->photo() && temp->photo()->isSelected())
            temp->photo()->setSelected(false);
    }
    QSet<QModelIndex> selected = newSel.toSet().subtract(oldSel.toSet());
    foreach (QModelIndex index, selected)
    {
        if (index.column() != LayersModelItem::NameString)
            continue;
        temp = static_cast<LayersModelItem*>(index.internalPointer());
        if (temp->photo() && !temp->photo()->isSelected())
            temp->photo()->setSelected(true);
    }
}
