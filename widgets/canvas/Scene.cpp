#include "Scene.h"
#include "QGraphicsEditionWidget.h"
#include "global.h"

// Qt
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMenu>
#include <QGraphicsSceneDragDropEvent>
#include <QStyleOptionGraphicsItem>
#include <QMap>
#include <QGraphicsWidget>
#include <qmath.h>
#include <QUndoCommand>

// KDE
#include <kapplication.h>
#include <kmessagebox.h>
#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

QColor Scene::OUTSIDE_SCENE_COLOR;

class KIPIPhotoFramesEditor::ScenePrivate
{
    enum
    {
        Rotation = 1,
        Moving = 2,
    };

    ScenePrivate(Scene * parent) :
        //m_edit_widget(new QGraphicsEditionWidget),
        m_parent(parent),
        m_pressed_item(0),
        m_selected_items_all_movable(true),
        m_selection_visible(true)
    {
        // Editing widget
        setMode(0);
        //QObject::connect(m_edit_widget, SIGNAL(deleteSelectedItems()), parent, SLOT(removeSelectedItems()));
        //parent->QGraphicsScene::addItem(m_edit_widget);
        //m_edit_widget->setZValue(1.0/0.0);

        // Background
        m_background = new QGraphicsRectItem(parent->sceneRect(), 0, parent);
        m_background->setZValue(-1.0/0.0);
        m_background->setBrush(Qt::white);
        m_background->setPen(QPen(Qt::transparent, 0));
    }

    void setMode(int mode)
    {
        m_mode = mode;
        //m_edit_widget->setRotationVisible(mode & Rotation);
        //m_edit_widget->reset();
    }

    // Parent scene
    QGraphicsScene * m_parent;
    // Background item
    QGraphicsRectItem * m_background;

    // Used for selecting items
    void deselectSelected()
    {
        m_selected_items_all_movable = true;
        foreach (AbstractPhoto * photo, m_selected_items.keys())
        {
            photo->setSelected(false);
            if (photo->hasFocus())
                photo->clearFocus();
        }
        m_selected_items.clear();
        m_selected_items_path = QPainterPath();
    }
    bool selectPressed()
    {
        if (m_pressed_item)
        {
            // Select if not selested
            if (!m_pressed_item->isSelected())
            {
                m_selected_items.insert(m_pressed_item, m_pressed_item->pos());
                m_selected_items_path = m_selected_items_path.united(m_pressed_item->mapToScene(m_pressed_item->shape()));
                m_selected_items_all_movable &= m_pressed_item->flags() & QGraphicsItem::ItemIsMovable;
                m_pressed_item->setSelected(true);
                setSelectionInitialPosition();
            }
            return true;
        }
        return false;
    }
    void focusPressed()
    {
        if (!m_pressed_item)
            return;
        // If is selectable and focusable try to set focus and post mousepressevent to it
        if (m_pressed_item->flags() & QGraphicsItem::ItemIsFocusable)
            m_pressed_item->setFocus(Qt::MouseFocusReason);
    }
    void setSelectionInitialPosition()
    {
        QMap<AbstractPhoto*,QPointF>::iterator it = m_selected_items.begin();
        while (it != m_selected_items.end())
        {
            it.value() = it.key()->pos();
            ++it;
        }
        m_selected_items_path_initial_pos = m_selected_items_path.boundingRect().topLeft();
    }
    bool wasMoved()
    {
        QMap<AbstractPhoto*,QPointF>::iterator it = m_selected_items.begin();
        while (it != m_selected_items.end())
        {
            if (it.value() != it.key()->pos())
                return true;
            ++it;
        }
        return false;
    }
    QMap<AbstractPhoto*,QPointF> m_selected_items;
    AbstractPhoto * m_pressed_item;
    QPainterPath m_selected_items_path;
    QPointF m_selected_items_path_initial_pos;
    bool m_selected_items_all_movable;
    bool m_selection_visible;
    QList<const char *> m_selection_filters;

    int m_mode;
    friend class Scene;
};

class KIPIPhotoFramesEditor::MoveItemsUndoCommand : public QUndoCommand
{
        QMap<AbstractPhoto*,QPointF> m_items;
        Scene * m_scene;
        bool done;
        static int num;
    public:
        MoveItemsUndoCommand(QMap<AbstractPhoto*,QPointF> items, Scene * scene, QUndoCommand * parent = 0) :
            QUndoCommand(i18n("Move items"), parent),
            m_items(items),
            m_scene(scene),
            done(true)
        {}
        virtual void redo();
        virtual void undo();
};
int KIPIPhotoFramesEditor::MoveItemsUndoCommand::num = 0;
void MoveItemsUndoCommand::redo()
{
    if (!done)
    {
        QMap<AbstractPhoto*,QPointF>::iterator it = m_items.begin();
        while(it != m_items.end())
        {
            QPointF temp = it.key()->pos();
            it.key()->setPos( it.value() );
            it.value() = temp;
            ++it;
        }
        done = !done;
        m_scene->calcSelectionBoundingRect();
    }
}
void MoveItemsUndoCommand::undo()
{
    if (done)
    {
        QMap<AbstractPhoto*,QPointF>::iterator it = m_items.begin();
        while(it != m_items.end())
        {
            QPointF temp = it.key()->pos();
            it.key()->setPos( it.value() );
            it.value() = temp;
            ++it;
        }
        done = !done;
        m_scene->calcSelectionBoundingRect();
    }
}

Scene::Scene(const QRectF & dimension, QObject * parent) :
    QGraphicsScene(dimension, parent),
    d(new ScenePrivate(this)),
    x_grid(0),
    y_grid(0),
    grid_visible(false),
    grid_item(0),
    grid_changed(true)
{
    if (!OUTSIDE_SCENE_COLOR.isValid())
    {
        QPalette pal = this->palette();
        OUTSIDE_SCENE_COLOR = pal.color(QPalette::Window);
        OUTSIDE_SCENE_COLOR.setAlpha(190);
    }

    // Mouse interaction mode
    setInteractionMode(DEFAULT_EDITING_MODE);

    // Create default grid
    setGrid(25,25);
    setGridVisible(false);

    // Signal connections
    connect(this, SIGNAL(selectionChanged()), this, SLOT(calcSelectionBoundingRect()));
}

//#####################################################################################################
Scene::~Scene()
{
    delete d;
}

//#####################################################################################################
void Scene::removeItem(AbstractPhoto * item)
{
    emit itemAboutToBeRemoved(item);
}

//#####################################################################################################
void Scene::removeItems(const QList<AbstractPhoto *> & items)
{
    emit itemsAboutToBeRemoved(items);
}

//#####################################################################################################
void Scene::enableItemsDrawing()
{
    //this->m_interaction_mode = Drawing;
    temp_path = QPainterPath();
    this->temp_widget = new QGraphicsPathItem(temp_path);
    //temp_widget->setZValue(zIndex++);
    QGraphicsScene::addItem(temp_widget);
}

//#####################################################################################################
void Scene::disableitemsDrawing()
{
    this->QGraphicsScene::removeItem(temp_widget);
    delete temp_widget;
    temp_widget = 0;
    m_interaction_mode = DEFAULT_EDITING_MODE;
}

//#####################################################################################################
void Scene::removeSelectedItems()
{
    removeItems(selectedItems());
}

//#####################################################################################################
void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    QGraphicsScene::contextMenuEvent(event);
    if (event->isAccepted())
        return;
    QMenu menu;
    QMenu * add = menu.addMenu("Add item");
    QAction * polygon = add->addAction("Polygon");
    connect(polygon,SIGNAL(triggered()),this,SLOT(enableItemsDrawing()));
    menu.exec(event->screenPos());
}

//#####################################################################################################
void Scene::keyPressEvent(QKeyEvent * event)
{
    if (d->m_selected_items.count() == 1 && (d->m_selected_items.begin().key()->hasFocus()))
    {
        this->setFocusItem(d->m_selected_items.begin().key());
        QGraphicsScene::keyPressEvent(event);
        event->setAccepted(true);
        return;
    }
    switch(event->key())
    {
        case Qt::Key_Delete:
            this->removeItems(selectedItems());
            event->setAccepted(true);
            break;
        case Qt::Key_Escape:
            disableitemsDrawing();
            break;
    }
    if (event->isAccepted())
        return;
    QGraphicsScene::keyPressEvent(event);
}

//#####################################################################################################
void Scene::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {

        // If moving enabled
        if (m_interaction_mode & Selecting)
        {
            this->calcSelectionBoundingRect();

            // Get initial selection position
            d->setSelectionInitialPosition();

            // If single selection mode, clear CTRL modifier
            if (selectionMode & SingleSelection)
                event->setModifiers(event->modifiers() & !Qt::ControlModifier);

            // Get pressed item
            d->m_pressed_item = dynamic_cast<AbstractPhoto*>(this->itemAt(event->scenePos()));

            // If event pos is not in current selection shape...
            if (!d->m_selected_items_path.contains(event->scenePos()) || !d->m_selected_items.contains(d->m_pressed_item))
            {
                // Clear focus from focused items
                if (this->focusItem())
                {
                    this->focusItem()->clearFocus();
                    d->m_pressed_item = 0;
                }
                // Or clear this selection
                else if (!(event->modifiers() & Qt::ControlModifier))
                    d->deselectSelected();
            }

            // Filtering selection
            if (d->m_pressed_item &&
                    d->m_selection_filters.count() &&
                    !d->m_selection_filters.contains( d->m_pressed_item->metaObject()->className() ))
                d->m_pressed_item = 0;

            // If there is VALID item to select...
            if (d->m_pressed_item)
            {
                // If not selectable -> deselect item
                if (!(d->m_pressed_item->flags() & QGraphicsItem::ItemIsSelectable))
                    d->m_pressed_item = 0;
                else
                {
                    // Send mousepressevent to the pressed item
                    event->setPos(d->m_pressed_item->mapFromScene(event->scenePos()));
                    event->setButtonDownPos(event->button(),
                                            d->m_pressed_item->mapFromScene(event->buttonDownScenePos(event->button())));
                    event->setLastPos(d->m_pressed_item->mapFromScene(event->lastScenePos()));
                    d->m_pressed_item->mousePressEvent(event);
                }
            }
            // If listeners should know scene press position
            else if (m_interaction_mode & MouseTracking)
                emit mousePressedPoint(event->buttonDownScenePos(event->button()));
        }
        event->setAccepted(d->m_pressed_item);
    }
    else
        QGraphicsScene::mousePressEvent(event);
}

//#####################################################################################################
void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (m_interaction_mode & Moving)
        {
            // Selecting pressed item
            event->setAccepted(d->selectPressed());

            // Moving items
            if (d->m_selected_items_all_movable)
            {
                // Calculate movement
                QPointF distance = event->scenePos() - event->buttonDownScenePos(Qt::LeftButton) + d->m_selected_items_path_initial_pos;
                if (event->modifiers() & Qt::ShiftModifier && this->isGridVisible())
                {
                    distance.setX(x_grid*round(distance.rx()/x_grid));
                    distance.setY(y_grid*round(distance.ry()/y_grid));
                }
                QPointF difference = d->m_selected_items_path.boundingRect().topLeft();
                          d->m_selected_items_path.translate(distance);
                d->m_selected_items_path.translate(-difference);
                difference = distance - difference;
                d->m_selected_items_path.translate(difference);
                foreach (AbstractPhoto * item, d->m_selected_items.keys())
                    item->setPos(item->pos() + difference);
            }
        }
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}

//#####################################################################################################
void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (m_interaction_mode & Selecting)
        {
            // Selecting pressed item
            event->setAccepted(d->selectPressed());

            if (m_interaction_mode & OneclickFocusItems)
                d->focusPressed();

            // Send mousereleaseevent to the released item
            if (d->m_pressed_item)
            {
                event->setPos(d->m_pressed_item->mapFromScene(event->scenePos()));
                event->setButtonDownPos(event->button(),
                                        d->m_pressed_item->mapFromScene(event->buttonDownScenePos(event->button())));
                event->setLastPos(d->m_pressed_item->mapFromScene(event->lastScenePos()));
                d->m_pressed_item->mouseReleaseEvent(event);
            }

            // Post move command to QUndoStack
            if ((m_interaction_mode & Moving) && d->wasMoved())
            {
                QUndoCommand * command = new MoveItemsUndoCommand(d->m_selected_items, this);
                PFE_PostUndoCommand(command);
            }
        }
    }
    else
        QGraphicsScene::mouseReleaseEvent(event);
}

//#####################################################################################################
void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        // In this app there is no difference between doubleClick and press events.
        // in Qt mouse events are alled in this order:        pressEvent -> releaseEvent -> doubleClickEvent -> releaseEvent
        // So for correct working second call of releaseEvent it is needed to call mousePressEvent here.
        this->mousePressEvent(event);

        // If selecting enabled -> focus item
        if (m_interaction_mode & Selecting)
            d->focusPressed();
    }
    else
        QGraphicsScene::mouseDoubleClickEvent(event);
}

//#####################################################################################################
void Scene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    // scene()->addItem(it);
    qDebug() << event->mimeData();
}

//#####################################################################################################
void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragMoveEvent(event);
}

//#####################################################################################################
void Scene::drawBackground(QPainter * painter, const QRectF & rect)
{
    QGraphicsScene::drawBackground(painter, rect.intersect(this->sceneRect()));
}

//#####################################################################################################
void Scene::drawForeground(QPainter * painter, const QRectF & rect)
{
    QGraphicsScene::drawForeground(painter, rect.intersect(this->sceneRect()));

    // Fill scene outside sceneRect with semi-transparent window color
    QPainterPath p;
    p.addRect(rect);
    QPainterPath s;
    s.addRect(this->sceneRect());
    painter->fillPath(p.subtracted(s), OUTSIDE_SCENE_COLOR);

    // Draw selected items shape
    if (isSelectionVisible())
    {
        this->calcSelectionBoundingRect();
        painter->save();
        painter->setPen(Qt::red);
        painter->setCompositionMode(QPainter::RasterOp_NotSourceAndNotDestination);
        painter->drawPath(d->m_selected_items_path);
        painter->restore();
    }
}

//#####################################################################################################
void Scene::setGrid(qreal x, qreal y)
{
    // Grid can't be 0
    if (x == 0 || y == 0)
        return;

    // Prevent recreation of unchanged grid
    if (x != this->x_grid || y != this->y_grid)
        grid_changed = true;

    if (!grid_item)
    {
        grid_item = new QGraphicsItemGroup(0,this);
        grid_item->setZValue(0);
        grid_item->setVisible(false);
        QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(0.5);
        grid_item->setGraphicsEffect(effect);
    }

    this->x_grid = x;
    this->y_grid = y;

    // Repaint only when x or y has changed
    if (grid_changed)
    {
        qreal width = sceneRect().width();
        qreal height = sceneRect().height();
        QList<QGraphicsItem*> children = grid_item->childItems();
        QList<QGraphicsItem*>::iterator it = children.begin();
        QGraphicsLineItem * temp;

        for (qreal i = x; i < width; i+=x)
        {
            if (it != children.end())
            {
                temp = static_cast<QGraphicsLineItem*>(*it);
                temp->setLine(i,0,i,height);
                ++it;
            }
            else
            {
                temp = new QGraphicsLineItem(i,0,i,height, 0, this);
                grid_item->addToGroup(temp);
            }
        }

        for (qreal i = y; i < height; i+=y)
        {
            if (it != children.end())
            {
                temp = static_cast<QGraphicsLineItem*>(*it);
                temp->setLine(0,i,width,i);
                ++it;
            }
            else
            {
                temp = new QGraphicsLineItem(0,i,width,i, 0, this);
                grid_item->addToGroup(temp);
            }
        }

        while (it != children.end())
        {
            grid_item->removeFromGroup(*it);
            delete *it;
        }

//        if (grid_visible)
//            updateChildernsGrid(x,y);
    }
}

//#####################################################################################################
void Scene::setGridVisible(bool visible)
{
    if (grid_visible == visible)
        return;
    grid_item->setVisible((grid_visible = visible));
//    if (visible)
//        updateChildernsGrid(x_grid,y_grid);
//    else
//        updateChildernsGrid(1,1);
}

//#####################################################################################################
bool Scene::isGridVisible()
{
    return this->grid_visible;
}

//#####################################################################################################
void Scene::setInteractionMode(int mode)
{
    m_interaction_mode = mode;
}

//#####################################################################################################
void Scene::setSelectionMode(SelectionMode selectionMode)
{
    switch(selectionMode)
    {
        case NoSelection:
            this->setSelectionArea(QPainterPath());
            this->selectionMode = selectionMode;
            break;
        case MultiSelection:
            this->selectionMode = selectionMode;
            break;
        case SingleSelection:
            this->setSelectionArea(QPainterPath());
            this->selectionMode = selectionMode;
            break;
    }
}

//#####################################################################################################
void Scene::addSelectingFilter(const QMetaObject & classMeta)
{
    d->m_selection_filters.push_back(classMeta.className());
}

//#####################################################################################################
void Scene::clearSelectingFilters()
{
    d->m_selection_filters.clear();
}

//#####################################################################################################
QDomNode Scene::toSvg(QDomDocument & document)
{
    QDomElement result = document.createElement("svg");
    result.setAttribute("xmlns","http://www.w3.org/2000/svg");
    result.setAttribute("width",QString::number(this->width()));
    result.setAttribute("height",QString::number(this->height()));
    QList<QGraphicsItem*> itemsList = this->items(Qt::AscendingOrder);
    foreach (QGraphicsItem * item, itemsList)
    {
        AbstractPhoto * photo = dynamic_cast<AbstractPhoto*>(item);
        if (photo)
            result.appendChild(photo->toSvg(document)); /// TODO : true (should be able to set to false!)
    }
    return result;
}

//#####################################################################################################
Scene * Scene::fromSvg(QDomElement & svgImage)
{
    if (svgImage.tagName() != "svg")
        return 0;

    // Scene dimension
    qreal width = svgImage.attribute("width").toDouble();
    qreal height = svgImage.attribute("width").toDouble();
    QRectF dimension(0,0,width,height);
    Scene * result = new Scene(dimension);

    // Create elements
    int errorsCount = 0;
    QDomNodeList children = svgImage.childNodes();
    for (int i = 0; i < children.count(); ++i)
    {
        QDomNode node = children.at(i);
        if (!node.isElement())
            continue;
        QDomElement element = node.toElement();
        QString itemClass = element.attribute("class");
        AbstractPhoto * item;
        if (itemClass == "PhotoItem")
            item = PhotoItem::fromSvg(element);
        else if (itemClass == "TextItem")
            item = TextItem::fromSvg(element);
        else
            item = 0;

        if (item)
        {
            result->addItem(item);
            item->setZValue(i+1);
        }
        else
            ++errorsCount;
    }

    // Show error message
    if (errorsCount)
    {
        KMessageBox::error(0,
                           i18n("Unable to read %1 element%2!",
                                QString::number(errorsCount).toAscii().constData(),
                                (errorsCount > 1 ? "s" : "")));
    }

    return result;
}

//#####################################################################################################
void Scene::updateSelection()
{
    //d->m_edit_widget->refresh();
}

//#####################################################################################################
void Scene::addItem(AbstractPhoto * item)
{
    if (item->scene() != this)
        this->QGraphicsScene::addItem(item);
    this->children.append(item);
    emit newItemAdded(item);
}

//#####################################################################################################
QList<AbstractPhoto*> Scene::selectedItems() const
{
    QList<AbstractPhoto*> result;
    const QList<QGraphicsItem*> & list = QGraphicsScene::selectedItems();
    foreach (QGraphicsItem * item, list)
        result << static_cast<AbstractPhoto*>(item);
    return result;
}

//#####################################################################################################
void Scene::calcSelectionBoundingRect()
{
    foreach (AbstractPhoto * item, d->m_selected_items.keys())
        if (!item->isSelected())
            d->m_selected_items.remove(item);

    d->m_selected_items_path = QPainterPath();
    QList<AbstractPhoto*> itemsList = this->selectedItems();
    foreach (AbstractPhoto * item, itemsList)
    {
        if (!d->m_selected_items.contains(item))
            d->m_selected_items.insert(item, item->pos());
        d->m_selected_items_path = d->m_selected_items_path.united(item->mapToScene(item->shape()));
    }
}

//#####################################################################################################
bool Scene::isSelectionVisible()
{
    return d->m_selection_visible;
}

//#####################################################################################################
void Scene::setSelectionVisible(bool isVisible)
{
    d->m_selection_visible = isVisible;
}
