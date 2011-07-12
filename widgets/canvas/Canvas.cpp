#include "Canvas.h"
#include "Scene.h"
#include "LayersModel.h"
#include "LayersModelItem.h"
#include "LayersSelectionModel.h"
#include "UndoRemoveItem.h"
#include "UndoMoveRowsCommand.h"
#include "UndoBorderChangeCommand.h"

#include <kapplication.h>
#include <kmessagebox.h>
#include <klocalizedstring.h>

#define MAX_SCALE_LIMIT 4
#define MIN_SCALE_LIMIT 0.5

using namespace KIPIPhotoFramesEditor;

Canvas::Canvas(const QSizeF & dimension, QWidget *parent) :
    QGraphicsView(parent),
    m_items_group(0),
    m_undo_stack(new QUndoStack(this))
{
    m_scene = new Scene(QRectF(QPointF(0,0), QSizeF(dimension)), this);
    m_model = new LayersModel(this);
    m_selmodel = new LayersSelectionModel(m_model, this);

    this->setupGUI();
    this->enableViewingMode();

    // Signals connection
    connect(m_scene, SIGNAL(newItemAdded(AbstractPhoto*)), this, SLOT(addItemToModel(AbstractPhoto*)));
    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect(m_scene, SIGNAL(itemAboutToBeRemoved(AbstractPhoto*)), this, SLOT(removeItem(AbstractPhoto*)));
    connect(m_scene, SIGNAL(itemsAboutToBeRemoved(QList<AbstractPhoto*>)), this, SLOT(removeItems(QList<AbstractPhoto*>)));
    connect(m_selmodel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
}

/** ###########################################################################################################################
 * Change selection mode
 #############################################################################################################################*/
void Canvas::setSelectionMode(SelectionMode mode)
{
    if (mode & Viewing)
    {
        this->setInteractive(false);
        this->setDragMode(QGraphicsView::ScrollHandDrag);
        m_scene->setSelectionMode(Scene::NoSelection);
        goto save;
    }
    else if (mode & MultiSelecting)
    {
        this->setInteractive(true);
        this->setDragMode(QGraphicsView::RubberBandDrag);
        m_scene->setSelectionMode(Scene::MultiSelection);
        goto save;
    }
    else if (mode & SingleSelcting)
    {
        this->setInteractive(true);
        this->setDragMode(QGraphicsView::ScrollHandDrag);
        m_scene->setSelectionMode(Scene::SingleSelection);
        goto save;
    }
    return;
    save:
        m_selection_mode = mode;
}

/** ###########################################################################################################################
 * Change interaction mode
 #############################################################################################################################*/
void Canvas::setInteractionMode(InteractionMode mode)
{
    if (!mode)
        m_interaction_mode = mode;
    else if (mode & BorderToolMode)
        m_interaction_mode = mode;
    else if (mode & ColorizeToolMode)
        m_interaction_mode = mode;
    else
        return;
    if (mode & SingleElementEditingMode)
        setSelectionMode(SingleSelcting);
}

void Canvas::addImage(const QImage & image)
{
    // Create & setup item
    PhotoItem * it = new PhotoItem(image,m_scene);
    it->setName(image.text("File").append(QString::number(m_model->rowCount())));
    it->setZValue(m_model->rowCount()+1);

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
 * Creates item move down command and push it to the undo stack
 #############################################################################################################################*/
void Canvas::moveRowsCommand(const QModelIndex & startIndex, int count, const QModelIndex & parentIndex, int move, const QModelIndex & destinationParent)
{
    int destination = startIndex.row();
    if (move > 0)
        destination += count + move;
    else if (move < 0)
        destination += move;
    else
        return;
    UndoMoveRowsCommand * undo = new UndoMoveRowsCommand(startIndex.row(), count, parentIndex, destination, destinationParent, m_model);
    m_undo_stack->push(undo);
}

/** ##########################################################################################################################
 * Move selected items up on scene & model. (Called by layers tree)
 #############################################################################################################################*/
void Canvas::moveSelectedRowsUp()
{
    QModelIndexList selectedIndexes = m_selmodel->selectedIndexes();
    if (!selectedIndexes.count())
    {
        #ifdef  QT_DEBUG
        qDebug() << "No items selected to move!" << selectedIndexes;
        #endif
        return;
    }
    // Check continuity of selection
    QModelIndexList::iterator it = selectedIndexes.begin();
    QModelIndex startIndex = *it;
    if (startIndex.isValid())
    {
        int minRow = startIndex.row();
        int maxRow = startIndex.row();
        int sumRows = startIndex.row();
        for(++it; it != selectedIndexes.end(); ++it)
        {
            if (it->column() != LayersModelItem::NameString)
                continue;
            if (startIndex.parent() != it->parent())
            {
                #ifdef  QT_DEBUG
                qDebug() << "Different parents of items!\n" << selectedIndexes;
                #endif
                return;
            }
            else if (!it->isValid())
            {
                #ifdef  QT_DEBUG
                qDebug() << "Invalid items!\n" << selectedIndexes;
                #endif
                return;
            }
            if (it->row() < minRow)
            {
                startIndex = *it;
                minRow = it->row();
            }
            if (it->row() > maxRow)
                maxRow = it->row();
            sumRows += it->row();
        }
        if ((((minRow+maxRow)*(maxRow-minRow+1))/2.0) != sumRows)
        {
            #ifdef  QT_DEBUG
            qDebug() << "Unordered items!\n" << selectedIndexes;
            #endif
            return;
        }
        if (minRow) // It means "is there any space before starting index to move selection"
            moveRowsCommand(startIndex, selectedIndexes.count(), startIndex.parent(), -1, startIndex.parent());
    }
    this->selectionChanged();
}

/** ##########################################################################################################################
 * Move selected items down on scene & model. (Called by layers tree)
 #############################################################################################################################*/
void Canvas::moveSelectedRowsDown()
{
    QModelIndexList selectedIndexes = m_selmodel->selectedIndexes();
    if (!selectedIndexes.count())
    {
        #ifdef  QT_DEBUG
        qDebug() << "No items selected to move!" << selectedIndexes;
        #endif
        return;
    }
    // Check continuity of selection
    QModelIndexList::iterator it = selectedIndexes.begin();
    QModelIndex startIndex = *it;
    if (startIndex.isValid())
    {
        int minRow = startIndex.row();
        int maxRow = startIndex.row();
        int sumRows = startIndex.row();
        for(++it; it != selectedIndexes.end(); ++it)
        {
            if (it->column() != LayersModelItem::NameString)
                continue;
            if (startIndex.parent() != it->parent())
            {
                #ifdef  QT_DEBUG
                qDebug() << "Different parents of items!\n" << selectedIndexes;
                #endif
                return;
            }
            else if (!it->isValid())
            {
                #ifdef  QT_DEBUG
                qDebug() << "Invalid items!\n" << selectedIndexes;
                #endif
                return;
            }
            if (it->row() < minRow)
            {
                startIndex = *it;
                minRow = it->row();
            }
            if (it->row() > maxRow)
                maxRow = it->row();
            sumRows += it->row();
        }
        if ((((minRow+maxRow)*(maxRow-minRow+1))/2.0) != sumRows)
        {
            #ifdef  QT_DEBUG
            qDebug() << "Unordered items!\n" << selectedIndexes;
            #endif
            return;
        }
        if (maxRow+1 < m_model->rowCount(startIndex.parent())) // It means "is there any space before starting index to move selection"
            moveRowsCommand(startIndex, selectedIndexes.count(), startIndex.parent(), 1, startIndex.parent());
    }
    this->selectionChanged();
}

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
void Canvas::removeSelectedRows()
{
    QModelIndexList selectedIndexes = m_selmodel->selectedRows();
    if (askAboutRemoving(selectedIndexes.count()))
    {
        if (selectedIndexes.count() > 1)
            beginRowsRemoving();
        foreach (QModelIndex index, selectedIndexes)
        {
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
    QModelIndexList oldSelected = m_selmodel->selectedIndexes();
    QModelIndexList newSelected = m_model->itemsToIndexes(selectedItems);
    foreach (QModelIndex index, oldSelected)
    {
        if (!newSelected.contains(index) && index.column() == LayersModelItem::NameString)
            m_selmodel->select(index, QItemSelectionModel::Rows | QItemSelectionModel::Deselect);
    }
    foreach(QModelIndex index, newSelected)
    {
        if (!m_selmodel->isSelected(index) && index.column() == LayersModelItem::NameString)
            m_selmodel->select(index, QItemSelectionModel::Rows | QItemSelectionModel::Select);
    }

    // Selection change signals
    selectedItems = m_scene->selectedItems();
    if (m_selection_mode & SingleSelcting)
    {
        if (selectedItems.count() == 1)
        {
            emit hasSelectionChanged(true);
            AbstractPhoto * item = selectedItems.at(0);
            // Specific signals emitting
            if (m_interaction_mode & BorderToolMode)
                emit setInitialValues(item->borderWidth(), item->borderCornersStyle(), item->borderColor());
        }
        else
            emit hasSelectionChanged(false);
    }
    else if (m_selection_mode & MultiSelecting)
        emit hasSelectionChanged(selectedItems.count());
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

/** ###########################################################################################################################
 * Border change command
 #############################################################################################################################*/
void Canvas::borderChangeCommand(qreal width, Qt::PenJoinStyle cornerStyle, const QColor & color)
{
    QList<AbstractPhoto*> selectedItem = m_scene->selectedItems();
    if (m_selection_mode & SingleSelcting && m_interaction_mode & BorderToolMode && selectedItem.count() == 1)
    {
        AbstractPhoto * item = selectedItem.at(0);
        UndoBorderChangeCommand * undo = new UndoBorderChangeCommand(item, width, cornerStyle, color);
        m_undo_stack->push(undo);
    }
}

/** ###########################################################################################################################
 * Connect widgets to canvas signals
 #############################################################################################################################*/
void Canvas::refreshWidgetConnections(bool isVisible)
{
    if (isVisible)
    {
        connect(this,SIGNAL(hasSelectionChanged(bool)),sender(),SLOT(setEnabled(bool)));
        emit hasSelectionChanged(m_scene->selectedItems().count());
    }
    else
        disconnect(this,SIGNAL(hasSelectionChanged(bool)),sender(),0);
}

/** ###########################################################################################################################
 * Appends new undo command
 #############################################################################################################################*/
void Canvas::newUndoCommand(QUndoCommand * command)
{
    m_undo_stack->push(command);
}

















void Canvas::setupGUI()
{
    this->setAcceptDrops(true);
    this->setAutoFillBackground(true);
    this->viewport()->setAutoFillBackground(false);
    this->setCacheMode(QGraphicsView::CacheBackground);
    this->setBackgroundBrush(QPalette().brush(QPalette::Window));
    /*this->setRenderHint(QPainter::Antialiasing);*/                                /// It causes worst quality!
    /*this->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);*/  /// It causes worst quality!

    this->setScene(m_scene);

    QImage img("/home/coder89/Desktop/routing.jpg");        /// TODO : Remove after finish
    img.setText("File","routing.jpg");
    this->addImage(img);                                /// TODO : Remove after finish
    this->addImage(img);                                /// TODO : Remove after finish
    this->addImage(img);                                /// TODO : Remove after finish
    this->addImage(img);                                /// TODO : Remove after finish
    this->addImage(img);                                /// TODO : Remove after finish
    this->addImage(img);                                /// TODO : Remove after finish
    this->addImage(img);                                /// TODO : Remove after finish
    this->addImage(img);                                /// TODO : Remove after finish
    this->addImage(img);                                /// TODO : Remove after finish
}

void Canvas::wheelEvent(QWheelEvent * event)
{
    // Scaling limitation
    static double factor = 1;
    if ( !scene() || (factor > MAX_SCALE_LIMIT && event->delta() > 0) || (factor < MIN_SCALE_LIMIT && event->delta() < 0) )
        return;

    double scaleFactor;
    if(event->delta() > 0)
        scaleFactor = (factor + 0.1) / factor;
    else
        scaleFactor = (factor - 0.1) / factor;

    scale(scaleFactor, scaleFactor);

    centerOn( mapToScene(event->pos()) );

    factor *= scaleFactor;
}

#undef MAX_SCALE_LIMIT
#undef MIN_SCALE_LIMIT
