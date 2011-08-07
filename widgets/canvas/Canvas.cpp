#include "Canvas.h"
#include "Scene.h"
#include "LayersModel.h"
#include "LayersModelItem.h"
#include "LayersSelectionModel.h"
#include "UndoMoveRowsCommand.h"
#include "UndoBorderChangeCommand.h"
#include "global.h"

#include <QPrinter>

#include <kapplication.h>
#include <kmessagebox.h>
#include <klocalizedstring.h>

#define MAX_SCALE_LIMIT 4
#define MIN_SCALE_LIMIT 0.5

using namespace KIPIPhotoFramesEditor;

Canvas::Canvas(const QSizeF & dimension, QWidget * parent) :
    QGraphicsView(parent)
{
    m_scene = new Scene(QRectF(QPointF(0,0), QSizeF(dimension)), this);
    this->init();
}

Canvas::Canvas(Scene * scene, QWidget * parent) :
    QGraphicsView(parent)
{
    Q_ASSERT(scene != 0);
    m_scene = scene;
    m_scene->setParent(this);
    this->setScene(m_scene);

    this->init();
}

/** ###########################################################################################################################
* Initialize Canvas object
#############################################################################################################################*/
void Canvas::init()
{
    m_is_saved = true;
    m_saved_on_index = 0;
    m_undo_stack = new QUndoStack(this);
    m_scale_factor = 1;

    this->setupGUI();
    this->enableViewingMode();
    this->prepareSignalsConnection();
}

/** ###########################################################################################################################
* Setup GUI of canvas widget
#############################################################################################################################*/
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
//    this->addImage(img);                                /// TODO : Remove after finish
//    this->addImage(img);                                /// TODO : Remove after finish
//    this->addImage(img);                                /// TODO : Remove after finish
//    this->addImage(img);                                /// TODO : Remove after finish
//    this->addImage(img);                                /// TODO : Remove after finish
//    this->addImage(img);                                /// TODO : Remove after finish
//    this->addImage(img);                                /// TODO : Remove after finish
    this->addImage(img);                                /// TODO : Remove after finish
    this->addText(QString::fromUtf8("\tJakiś tam sobie tekst \n asoiufhasuhf iusd hggwsvbizuds \n iasgfgauu\nasiuf"));
    this->addText(QString::fromUtf8("Jakiś tam sobie tekst \n asoiufhasuhf iusd hggwsvbizuds \n iasgfgauu\nasiuf"));
}

/** ###########################################################################################################################
* Connect signals to slots
#############################################################################################################################*/
void Canvas::prepareSignalsConnection()
{
    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect(m_scene, SIGNAL(itemAboutToBeRemoved(AbstractPhoto*)), this, SLOT(removeItem(AbstractPhoto*)));
    connect(m_scene, SIGNAL(itemsAboutToBeRemoved(QList<AbstractPhoto*>)), this, SLOT(removeItems(QList<AbstractPhoto*>)));
    connect(m_scene->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
    connect(m_undo_stack, SIGNAL(indexChanged(int)), this, SLOT(isSavedChanged(int)));
    connect(m_undo_stack, SIGNAL(cleanChanged(bool)), this, SLOT(isSavedChanged(bool)));
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
        this->setDragMode(QGraphicsView::NoDrag);
        m_scene->setSelectionMode(Scene::SingleSelection);
        goto save;
    }
    return;
    save:
        m_selection_mode = mode;
}

/** ###########################################################################################################################
 * Data model connected with this canvas (in fact this is a Scene's model)
 #############################################################################################################################*/
LayersModel * Canvas::model() const
{
    return m_scene->model();
}

/** ###########################################################################################################################
 * Data selection model connected with this canvas (in fact this is a Scene's selection model)
 #############################################################################################################################*/
LayersSelectionModel * Canvas::selectionModel() const
{
    return m_scene->selectionModel();
}

/** ###########################################################################################################################
 * Add new image from QImage object
 #############################################################################################################################*/
void Canvas::addImage(const QImage & image)
{
    // Create & setup item
    PhotoItem * it = new PhotoItem(image);
    it->setName(image.text("File").append(QString::number(model()->rowCount())));

    // Add item to scene & model
    m_scene->addItem(it);
}

/** ###########################################################################################################################
 * Add new text item
 #############################################################################################################################*/
void Canvas::addText(const QString & text)
{
    // Create & setup item
    TextItem * it = new TextItem(text);

    // Add item to scene & model
    m_scene->addItem(it);
}

/** ###########################################################################################################################
 * Used when new item has been created and needs to be added to the scene and to the model
 #############################################################################################################################*/
void Canvas::addNewItem(AbstractPhoto * item)
{
    if (!item)
        return;

    m_scene->addItem(item);

    m_scene->clearSelection();
    m_scene->clearFocus();

    item->setSelected(true);
    item->setFocus( Qt::OtherFocusReason );
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
    UndoMoveRowsCommand * undo = new UndoMoveRowsCommand(startIndex.row(), count, parentIndex, destination, destinationParent, model());
    m_undo_stack->push(undo);
}

/** ##########################################################################################################################
 * Move selected items up on scene & model. (Called by layers tree)
 #############################################################################################################################*/
void Canvas::moveSelectedRowsUp()
{
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
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
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
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
        if (maxRow+1 < model()->rowCount(startIndex.parent())) // It means "is there any space before starting index to move selection"
            moveRowsCommand(startIndex, selectedIndexes.count(), startIndex.parent(), 1, startIndex.parent());
    }
    this->selectionChanged();
}

/** ##########################################################################################################################
 * Remove from model ONE item removed from scene
 #############################################################################################################################*/
void Canvas::removeItem(AbstractPhoto * item)
{
    if (item)
        m_scene->removeItem(item);
}

/** ##########################################################################################################################
 * Remove from model items removed from scene
 #############################################################################################################################*/
void Canvas::removeItems(const QList<AbstractPhoto*> & items)
{
    m_scene->removeItems(items);
}

/** ##########################################################################################################################
 * Remove from scene items removed from layert tree.
 #############################################################################################################################*/
void Canvas::removeSelectedRows()
{
    QList<AbstractPhoto*> items;
    QModelIndexList selectedIndexes = selectionModel()->selectedRows();
    foreach (QModelIndex index, selectedIndexes)
        items << static_cast<LayersModelItem*>(index.internalPointer())->photo();
    m_scene->removeItems(items);
}

/** ###########################################################################################################################
 * Synchronize scene's selection with model's selection
 #############################################################################################################################*/
void Canvas::selectionChanged()
{
    QList<AbstractPhoto*> selectedItems = m_scene->selectedItems();
    QModelIndexList oldSelected = selectionModel()->selectedIndexes();
    QModelIndexList newSelected = model()->itemsToIndexes(selectedItems);
    foreach (QModelIndex index, oldSelected)
    {
        if (!newSelected.contains(index) && index.column() == LayersModelItem::NameString)
            selectionModel()->select(index, QItemSelectionModel::Rows | QItemSelectionModel::Deselect);
    }
    foreach(QModelIndex index, newSelected)
    {
        if (!selectionModel()->isSelected(index) && index.column() == LayersModelItem::NameString)
            selectionModel()->select(index, QItemSelectionModel::Rows | QItemSelectionModel::Select);
    }

    // Selection change signals
    selectedItems = m_scene->selectedItems();
    if (m_selection_mode & SingleSelcting)
    {
        if (selectedItems.count() == 1)
        {
            AbstractPhoto * item = selectedItems.at(0);
            emit hasSelectionChanged(true);
            emit selectedItem(item);
        }
        else
        {
            emit hasSelectionChanged(false);
            emit selectedItem(NULL);
        }
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
 * Sets selecting mode
 #############################################################################################################################*/
void Canvas::enableDefaultSelectionMode()
{
    m_scene->setInteractionMode(Scene::Selecting | Scene::Moving);
    setSelectionMode(MultiSelecting);
    this->unsetCursor();
    m_scene->clearSelectingFilters();
}

/** ###########################################################################################################################
 * Sets viewing mode
 #############################################################################################################################*/
void Canvas::enableViewingMode()
{
    m_scene->setInteractionMode(Scene::View);
    setSelectionMode(Viewing);
    this->unsetCursor();
    m_scene->clearSelectingFilters();
}

/** ###########################################################################################################################
 * Sets effects editing mode
 #############################################################################################################################*/
void Canvas::enableEffectsEditingMode()
{
    m_scene->setInteractionMode(Scene::Selecting);
    setSelectionMode(SingleSelcting);
    this->setCursor(QCursor(Qt::ArrowCursor));
    m_scene->clearSelectingFilters();
    m_scene->addSelectingFilter(PhotoItem::staticMetaObject);
}

/** ###########################################################################################################################
 * Sets text editing mode
 #############################################################################################################################*/
void Canvas::enableTextEditingMode()
{
    m_scene->setInteractionMode(Scene::Selecting | Scene::MouseTracking | Scene::OneclickFocusItems);
    setSelectionMode(SingleSelcting);
    this->setCursor(Qt::CrossCursor);
    m_scene->clearSelectingFilters();
    m_scene->addSelectingFilter(TextItem::staticMetaObject);
}

/** ###########################################################################################################################
 * Sets rotating mode
 #############################################################################################################################*/
void Canvas::enableRotateEditingMode()
{
    m_scene->setInteractionMode(Scene::Selecting | Scene::Rotating);
    setSelectionMode(SingleSelcting);
    this->setCursor(Qt::ArrowCursor);
    m_scene->clearSelectingFilters();
}

/** ###########################################################################################################################
 * Sets borders editing mode
 #############################################################################################################################*/
void Canvas::enableBordersEditingMode()
{
    m_scene->setInteractionMode(Scene::Selecting);
    setSelectionMode(SingleSelcting);
    this->setCursor(Qt::ArrowCursor);
    m_scene->clearSelectingFilters();
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

/** ###########################################################################################################################
 * Wheel event - scales the canvas
 #############################################################################################################################*/
void Canvas::wheelEvent(QWheelEvent * event)
{
    int steps = event->delta() / 120;
    double scaleFactor;
    scaleFactor = (m_scale_factor + steps * 0.1) / m_scale_factor;
    scale(scaleFactor, event->pos());
}

/** ###########################################################################################################################
 * Converts canvas to SVG image
 #############################################################################################################################*/
QDomDocument Canvas::toSvg() const
{
    QDomDocument result(" svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"");
    result.appendChild(m_scene->toSvg(result));
    return result;
}

/** ###########################################################################################################################
 * Loads canvas state from SVG file
 #############################################################################################################################*/
Canvas * Canvas::fromSvg(QDomDocument & document)
{
    Canvas * result = 0;
    QDomNodeList childs = document.childNodes();
    if (childs.count())
    {
        int i = 0;
        QDomNode node;
        QDomElement element;
        while (childs.count() > i)
        {
            node = childs.at(i++);
            if (node.isElement())
            {
                element = node.toElement();
                if (element.tagName() == "svg")
                    break;
                else
                    element = QDomElement();
            }
        }
        if (!element.isNull())
        {
            qreal width = element.attribute("width").toDouble();
            qreal height = element.attribute("width").toDouble();
            qDebug() << width << height;
            QSizeF dimension(width,height);
            if (dimension.isValid())
            {
                Scene * scene = Scene::fromSvg(element);
                if (scene)
                    result = new Canvas(scene);
            }
        }
    }
    return result;
}

/** ###########################################################################################################################
 * Scales canvas view by the selected factor
 #############################################################################################################################*/
void Canvas::scale(qreal factor, const QPoint & center)
{
    // Scaling limitation
    if (factor <= 0 || !scene() || (m_scale_factor*factor <= 0.1 && factor < 1))
        return;

    QGraphicsView::scale(factor, factor);

    if (center.isNull())
        centerOn( m_scene->sceneRect().center() );
    else
        centerOn( mapToScene(center) );

    m_scale_factor *= factor;
}

/** ###########################################################################################################################
 * Scales canvas view to fit in rect
 #############################################################################################################################*/
void Canvas::scale(const QRect & rect)
{
    QSize viewSize = rect.size();
    QSizeF sceneSize = m_scene->sceneRect().size();
    qreal xFactor = viewSize.width() / sceneSize.width();
    qreal yFactor = viewSize.height() / sceneSize.height();
    qreal newFactor;
    if (xFactor > 1 && yFactor > 1)
        newFactor = xFactor > yFactor ? xFactor : yFactor;
    else
        newFactor = xFactor < yFactor ? xFactor : yFactor;

    this->scale(newFactor);
    qDebug() << "scaled" << newFactor;
}

/** ###########################################################################################################################
 * Returns file object connected with canvas
 #############################################################################################################################*/
KUrl Canvas::file() const
{
    return m_file;
}

/** ###########################################################################################################################
 * Sets the file connected with canvas
 #############################################################################################################################*/
void Canvas::setFile(const KUrl & file)
{
    if (file.isValid() && !file.isEmpty())
        m_file = file;
}

/** ###########################################################################################################################
 * Sets the file connected with canvas
 #############################################################################################################################*/
QString Canvas::save(const KUrl & fileUrl, bool setAsDefault)
{
    KUrl tempFile = fileUrl;
    if (fileUrl.isEmpty() || !fileUrl.isValid())
    {
        if (m_file.isEmpty() || !m_file.isValid())
            return i18n("Invalid file path.");
        tempFile = m_file;
    }
    QFile file(tempFile.path());
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        file.write(this->toSvg().toString().toAscii());
        file.close();
        if (setAsDefault)
            m_file = tempFile;
        m_is_saved = true;
        m_saved_on_index = m_undo_stack->index();
        emit savedStateChanged();
        return QString();
    }
    else
        return file.errorString();
}

/** ###########################################################################################################################
 * Check if canvas is saved
 #############################################################################################################################*/
bool Canvas::isSaved()
{
    return m_is_saved;
}

/** ###########################################################################################################################
 * Controls changes on cavnas (based on QUndoStack state)
 #############################################################################################################################*/
void Canvas::isSavedChanged(int currentCommandIndex)
{
    m_is_saved = (m_saved_on_index == currentCommandIndex);
    emit savedStateChanged();
}

/** ###########################################################################################################################
 * Controls changes on cavnas (based on QUndoStack state)
 #############################################################################################################################*/
void Canvas::isSavedChanged(bool isStackClean)
{
    if (isStackClean)
        m_is_saved |= isStackClean;
    else
        m_is_saved = (m_saved_on_index == m_undo_stack->index());
    emit savedStateChanged();
}

#include <QtSvg/QSvgGenerator>

/** ###########################################################################################################################
 * Draws canvas content onto the QPaintDevice
 #############################################################################################################################*/
void Canvas::renderCanvas(QPaintDevice * device)
{
    if (scene())
    {
        scene()->setSelectionVisible(false);
        bool isGridVisible = scene()->isGridVisible();
        scene()->setGridVisible(false);

        QSvgGenerator gen;
        gen.setFileName("/home/coder89/Desktop/dupa.svg");
        gen.setViewBox(scene()->sceneRect());
        QPainter p2(&gen);
        scene()->render(&p2, scene()->sceneRect(), scene()->sceneRect());

        scene()->setSelectionVisible(false);
        QPainter p(device);
        scene()->render(&p, scene()->sceneRect(), scene()->sceneRect());
        p.end();

        scene()->setSelectionVisible(true);
        scene()->setGridVisible(isGridVisible);
    }
}

/** ###########################################################################################################################
 * Draws canvas content onto the printer
 #############################################################################################################################*/
void Canvas::renderCanvas(QPrinter * device)
{
    renderCanvas(static_cast<QPaintDevice*>(device));
}

#undef MAX_SCALE_LIMIT
#undef MIN_SCALE_LIMIT
