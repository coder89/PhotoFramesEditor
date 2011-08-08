#include "Scene.h"
#include "QGraphicsEditionWidget.h"
#include "global.h"
#include "QGraphicsRotationItem.h"

#include "LayersModel.h"
#include "LayersModelItem.h"
#include "LayersSelectionModel.h"

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
#include <QtAlgorithms>

// KDE
#include <kapplication.h>
#include <kmessagebox.h>
#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

QColor Scene::OUTSIDE_SCENE_COLOR;

class KIPIPhotoFramesEditor::ScenePrivate
{
    ScenePrivate(Scene * parent) :
        parent(parent),
        model(new LayersModel(parent)),
        selection_model(new LayersSelectionModel(model, parent)),
        m_pressed_item(0),
        m_selected_items_all_movable(true),
        m_selection_visible(true),
        m_rot_item(0)
    {
        // Background
        m_background = new QGraphicsRectItem(parent->sceneRect(), 0, parent);
        m_background->setZValue(-1.0/0.0);
        m_background->setBrush(Qt::white);
        m_background->setPen(QPen(Qt::transparent, 0));
    }

    QList<QGraphicsItem*> itemAtPosition(const QPointF & scenePos, QWidget * widget)
    {
        qDebug() << widget;
        QGraphicsView * view = widget ? qobject_cast<QGraphicsView*>(widget->parentWidget()) : 0;
        qDebug() << "itemsAtPosition";
        if (!view)
            return parent->items(scenePos, Qt::IntersectsItemShape, Qt::DescendingOrder, QTransform());
        qDebug() << "has view";
        const QRectF pointRect(scenePos, QSizeF(1, 1));
        if (!view->isTransformed())
            return parent->items(pointRect, Qt::IntersectsItemShape, Qt::DescendingOrder);
        qDebug() << "has transformation";
        const QTransform viewTransform = view->viewportTransform();
        return parent->items(pointRect, Qt::IntersectsItemShape, Qt::DescendingOrder, viewTransform);
    }
    AbstractItemInterface * itemAt(const QPointF & scenePos, QWidget * widget)
    {
        QList<QGraphicsItem*> items = itemAtPosition(scenePos, widget);
        return items.count() ? dynamic_cast<AbstractItemInterface*>(items.first()) : 0;
    }
    void sendPressEventToItem(AbstractItemInterface * item, QGraphicsSceneMouseEvent * event)
    {
        // Send mousepressevent to the pressed item
        event->setPos(item->mapFromScene(event->scenePos()));
        event->setButtonDownPos(event->button(),
                                item->mapFromScene(event->buttonDownScenePos(event->button())));
        event->setLastPos(item->mapFromScene(event->lastScenePos()));
        item->mousePressEvent(event);
    }
    void sendMoveEventToItem(AbstractItemInterface * item, QGraphicsSceneMouseEvent * event)
    {
        // Send mousepressevent to the pressed item
        event->setPos(item->mapFromScene(event->scenePos()));
        event->setButtonDownPos(event->button(),
                                item->mapFromScene(event->buttonDownScenePos(event->button())));
        event->setLastPos(item->mapFromScene(event->lastScenePos()));
        item->mouseMoveEvent(event);
    }
    void sendReleaseEventToItem(AbstractItemInterface * item, QGraphicsSceneMouseEvent * event)
    {
        // Send mousepressevent to the pressed item
        event->setPos(item->mapFromScene(event->scenePos()));
        event->setButtonDownPos(event->button(),
                                item->mapFromScene(event->buttonDownScenePos(event->button())));
        event->setLastPos(item->mapFromScene(event->lastScenePos()));
        item->mouseReleaseEvent(event);
    }
    void sendDoubleClickEventToItem(AbstractItemInterface * item, QGraphicsSceneMouseEvent * event)
    {
        // Send mousepressevent to the pressed item
        event->setPos(item->mapFromScene(event->scenePos()));
        event->setButtonDownPos(event->button(),
                                item->mapFromScene(event->buttonDownScenePos(event->button())));
        event->setLastPos(item->mapFromScene(event->lastScenePos()));
        item->mouseDoubleClickEvent(event);
    }

    // Parent scene
    QGraphicsScene * parent;
    // Scene's model
    LayersModel * model;
    LayersSelectionModel * selection_model;
    // Background item
    QGraphicsRectItem * m_background;

    // Used for selecting items
    void deselectSelected()
    {
        m_selected_items_all_movable = true;
        foreach (AbstractItemInterface * photo, m_selected_items.keys())
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
        if (m_pressed_item &&
            m_pressed_item != m_rot_item)
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
        QMap<AbstractItemInterface*,QPointF>::iterator it = m_selected_items.begin();
        while (it != m_selected_items.end())
        {
            it.value() = it.key()->pos();
            ++it;
        }
        m_selected_items_path_initial_pos = m_selected_items_path.boundingRect().topLeft();
    }
    bool wasMoved()
    {
        QMap<AbstractItemInterface*,QPointF>::iterator it = m_selected_items.begin();
        while (it != m_selected_items.end())
        {
            if (it.value() != it.key()->pos())
                return true;
            ++it;
        }
        return false;
    }
    QMap<AbstractItemInterface*,QPointF> m_selected_items;
    AbstractItemInterface * m_pressed_item;
    QPainterPath m_selected_items_path;
    QPointF m_selected_items_path_initial_pos;
    bool m_selected_items_all_movable;
    bool m_selection_visible;
    QList<const char *> m_selection_filters;

    // Used for rotating items
    QGraphicsRotationItem * m_rot_item;

    friend class Scene;
};

class KIPIPhotoFramesEditor::AddItemsCommand : public QUndoCommand
{
        QList<AbstractPhoto*> items;
        int position;
        Scene * scene;
        bool done;
    public:
        AddItemsCommand(AbstractPhoto * item, int position, Scene * scene, QUndoCommand * parent = 0) :
            QUndoCommand(parent),
            position(position),
            scene(scene),
            done(false)
        {
            items << item;
        }
        AddItemsCommand(const QList<AbstractPhoto*> & items, int position, Scene * scene, QUndoCommand * parent = 0) :
            QUndoCommand(parent),
            items(items),
            position(position),
            scene(scene),
            done(false)
        {}
        ~AddItemsCommand()
        {
            if (done)
                return;
            foreach (AbstractPhoto * item, items)
                if (!item->scene())
                    item->deleteLater();
            items.clear();
        }
        virtual void redo()
        {
            foreach (AbstractPhoto * item, items)
                scene->QGraphicsScene::addItem(item);
            scene->model()->insertItems(items, position);
            done = true;
        }
        virtual void undo()
        {
            foreach (AbstractPhoto * item, items)
                scene->QGraphicsScene::removeItem(item);
            scene->model()->removeRows(position, items.count());
            done = false;
        }
};
class KIPIPhotoFramesEditor::MoveItemsCommand : public QUndoCommand
{
        QMap<AbstractItemInterface*,QPointF> m_items;
        Scene * m_scene;
        bool done;
    public:
        MoveItemsCommand(QMap<AbstractItemInterface*,QPointF> items, Scene * scene, QUndoCommand * parent = 0) :
            QUndoCommand(i18n("Move items"), parent),
            m_items(items),
            m_scene(scene),
            done(true)
        {}
        virtual void redo()
        {
            if (!done)
            {
                QMap<AbstractItemInterface*,QPointF>::iterator it = m_items.begin();
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
        virtual void undo()
        {
            if (done)
            {
                QMap<AbstractItemInterface*,QPointF>::iterator it = m_items.begin();
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
};
class KIPIPhotoFramesEditor::RemoveItemsCommand : public QUndoCommand
{
        AbstractPhoto * item;
        int item_row;
        AbstractPhoto * item_parent;
        Scene * m_scene;
        bool done;
    public:
        RemoveItemsCommand(AbstractPhoto * item, Scene * scene, QUndoCommand * parent = 0) :
            QUndoCommand(QString("Remove item"), parent),
            item(item),
            m_scene(scene),
            done(false)
        {
            item_parent = dynamic_cast<AbstractPhoto*>(item->parentItem());
        }
        ~RemoveItemsCommand()
        {
            if (done)
            {
                if (item && !item->scene() && !item->parentItem())
                    delete item;
            }
        }
        virtual void redo()
        {
            QPersistentModelIndex parentIndex = QPersistentModelIndex(m_scene->model()->findIndex( item_parent ));
            if (item_parent && !(parentIndex.isValid() && item_parent->scene()))
                return;

            // Remove from model
            QModelIndex itemIndex = m_scene->model()->findIndex(item, parentIndex);
            item_row = itemIndex.row();
            if (itemIndex.isValid())
                m_scene->model()->removeRow(item_row, parentIndex);

            // Remove from scene
            if (item->scene() == m_scene)
                m_scene->QGraphicsScene::removeItem(item);
            done = true;
        }
        virtual void undo()
        {
            if (!done)
                return;

            // Add to scene
            if (item->scene() != m_scene)
                m_scene->QGraphicsScene::addItem( item );
            item->setParentItem( item_parent );

            // Add to model
            QPersistentModelIndex parentIndex = QPersistentModelIndex( m_scene->model()->findIndex( item_parent ) );
            if (!m_scene->model()->hasIndex(item_row, 0, parentIndex) ||
                    static_cast<LayersModelItem*>(m_scene->model()->index(item_row, 0, parentIndex).internalPointer())->photo() != item)
            {
                if (m_scene->model()->insertRow(item_row, parentIndex))
                {
                    static_cast<LayersModelItem*>(m_scene->model()->index(item_row, 0, parentIndex).internalPointer())->setPhoto(item);
                    // Add items children to model
                    appendChild(item, m_scene->model()->index(item_row, 0, parentIndex));
                }
            }
            done = false;
        }
    private:
        static bool compareGraphicsItems(QGraphicsItem * i1, QGraphicsItem * i2)
        {
            if ((i1 && i2) && (i1->zValue() < i2->zValue()))
                return true;
            return false;
        }
        void appendChild(AbstractPhoto * item, const QModelIndex & parent)
        {
            QList<QGraphicsItem*> items = item->childItems();
            if (items.count())
            {
                // Sort using z-Values (z-Value == models row)
                qSort(items.begin(), items.end(), KIPIPhotoFramesEditor::RemoveItemsCommand::compareGraphicsItems);
                int i = 0;
                foreach (QGraphicsItem * childItem, items)
                {
                    AbstractPhoto * photo = dynamic_cast<AbstractPhoto*>(childItem);
                    if (photo)
                    {
                        if (m_scene->model()->insertRow(i,parent))
                        {
                            static_cast<LayersModelItem*>(m_scene->model()->index(i, 0, parent).internalPointer())->setPhoto(photo);
                            this->appendChild(photo, m_scene->model()->index(i, 0, parent));
                            ++i;
                        }
                    }
                }
            }
        }
};
class KIPIPhotoFramesEditor::RotateItemsCommand : public QUndoCommand
{
    QList<AbstractItemInterface*> items;
    QTransform transform;
    QPointF rotationPoint;
    qreal angle;
    bool done;
public:
    RotateItemsCommand(const QList<AbstractItemInterface*> & items, const QPointF & rotationPoint, qreal angle, QUndoCommand * parent = 0) :
        QUndoCommand(parent),
        items(items),
        rotationPoint(rotationPoint),
        angle(angle),
        done(true)
    {
        qDebug() << "create rot command";
        transform.translate(this->rotationPoint.rx(), this->rotationPoint.ry());
        transform.rotate(angle);
        transform.translate(-this->rotationPoint.rx(), -this->rotationPoint.ry());
    }
    virtual void redo()
    {
        if (done)
            return;
        qDebug() << "redo rot";
        foreach (AbstractItemInterface * item, items)
        {
            QRectF updateRect = item->mapRectToScene(item->boundingRect());
            QTransform rotated = item->transform() * transform;
            item->setTransform(rotated);
            updateRect = updateRect.united( item->mapRectToScene(item->boundingRect()) );
            if (item->scene())
                item->scene()->invalidate(updateRect);
        }
        items.first()->scene()->clearSelection();
        done = true;
    }
    virtual void undo()
    {
        if (!done)
            return;
        qDebug() << "undo rot";
        foreach (AbstractItemInterface * item, items)
        {
            QRectF updateRect = item->mapRectToScene(item->boundingRect());
            QTransform rotated = item->transform() * transform.inverted();
            item->setTransform(rotated);
            updateRect = updateRect.united( item->mapRectToScene(item->boundingRect()) );
            if (item->scene())
                item->scene()->invalidate(updateRect);
        }
        items.first()->scene()->clearSelection();
        done = false;
    }
};

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

    // Indexing method
    this->setItemIndexMethod(QGraphicsScene::NoIndex);

    // Signal connections
    connect(this, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}

//#####################################################################################################
Scene::~Scene()
{
    delete d;
}

//#####################################################################################################
LayersModel * Scene::model() const
{
    return d->model;
}

//#####################################################################################################
LayersSelectionModel * Scene::selectionModel() const
{
    return d->selection_model;
}

//#####################################################################################################
void Scene::removeItem(AbstractPhoto * item)
{
    if (!askAboutRemoving(1))
        return;
    QUndoCommand * command = new RemoveItemsCommand(item, this);
    PFE_PostUndoCommand(command);
}

//#####################################################################################################
void Scene::removeItems(const QList<AbstractPhoto *> & items)
{
    if (!askAboutRemoving(items.count()))
        return;
    QUndoCommand * command;
    QUndoCommand * parent = 0;
    if (items.count() > 1)
        parent = new QUndoCommand("Remove items");
    foreach (AbstractPhoto * item, items)
        command = new RemoveItemsCommand(item, this, parent);
    if (parent)
        PFE_PostUndoCommand(parent);
    else
        PFE_PostUndoCommand(command);
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
            //disableitemsDrawing();
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
            d->m_pressed_item = d->itemAt(event->scenePos(), event->widget());

            // If it is rotation widget
            if ((m_interaction_mode & Rotating) && d->m_pressed_item == d->m_rot_item)
            {
                d->sendPressEventToItem(d->m_pressed_item, event);
                return;
            }

            // Test if this is a photo/text item
            d->m_pressed_item = dynamic_cast<AbstractPhoto*>(d->m_pressed_item);

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
                    d->sendPressEventToItem(d->m_pressed_item, event);
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
        if (d->m_pressed_item)
            d->sendMoveEventToItem(d->m_pressed_item, event);

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
                foreach (AbstractItemInterface * item, d->m_selected_items.keys())
                    item->setPos(item->pos() + difference);
            }
        }
    }
    else
        ;//QGraphicsScene::mouseMoveEvent(event);
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
                d->sendReleaseEventToItem(d->m_pressed_item, event);

            // Post move command to QUndoStack
            if ((m_interaction_mode & Moving) && d->wasMoved())
            {
                QUndoCommand * command = new MoveItemsCommand(d->m_selected_items, this);
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

        QList<QGraphicsItem*> toRemove;
        while (it != children.end())
            toRemove.append(*(it++));
        while (toRemove.count())
        {
            QGraphicsItem * temp = toRemove.takeAt(0);
            grid_item->removeFromGroup(temp);
            delete temp;
        }
    }
}

//#####################################################################################################
void Scene::setGridVisible(bool visible)
{
    if (grid_visible == visible)
        return;
    grid_item->setVisible((grid_visible = visible));
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
    setRotationWidgetVisible(mode & Rotating);
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
void Scene::setRotationWidgetVisible(bool isVisible)
{
    if (d->m_rot_item)
    {
        this->QGraphicsScene::removeItem(d->m_rot_item);
        d->m_rot_item->deleteLater();
        d->m_rot_item = 0;
    }

    if (isVisible)
    {
        if (!d->m_rot_item)
        {
            d->m_rot_item = new QGraphicsRotationItem();
            connect(d->m_rot_item, SIGNAL(rotationChanged(QPointF,qreal)), this, SLOT(rotateSelectedItems(QPointF,qreal)));
            connect(d->m_rot_item, SIGNAL(rotationFinished(QPointF,qreal)), this, SLOT(rotationCommand(QPointF,qreal)));
        }
        d->m_rot_item->setZValue(1.0/0.0);
        this->QGraphicsScene::addItem(d->m_rot_item);
        if (d->m_selected_items.count())
            d->m_rot_item->setPos(d->m_selected_items_path.boundingRect().center());
        else
            d->m_rot_item->hide();
    }
}

//#####################################################################################################
qreal Scene::gridHorizontalDistance() const
{
    return this->x_grid;
}

//#####################################################################################################
qreal Scene::gridVerticalDistance() const
{
    return this->y_grid;
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
void Scene::addItem(AbstractPhoto * item)
{
    // Prevent multiple addition of the item
    if (item->scene() == this)
        return;

    QModelIndexList selectedIndexes = d->selection_model->selectedIndexes();
    unsigned insertionRow = -1;
    foreach (QModelIndex index, selectedIndexes)
    {
        if (index.column() != LayersModelItem::NameString)
            continue;
        if (insertionRow > (unsigned)index.row())
            insertionRow = index.row();
    }
    if (insertionRow == (unsigned)-1)
        insertionRow = 0;
    QUndoCommand * command = new AddItemsCommand(item, insertionRow, this);
    PFE_PostUndoCommand(command);
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
void Scene::updateSelection()
{
    foreach (AbstractItemInterface * item, d->m_selected_items.keys())
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

    if (m_interaction_mode & Rotating)
    {
        if (d->m_selected_items.count() == 1)
        {
            AbstractItemInterface * item = d->m_selected_items.keys().first();
            d->m_rot_item->initRotation(d->m_selected_items_path, item->boundingRect().center() * item->transform());
            d->m_rot_item->show();
        }
        else
            d->m_rot_item->hide();
    }
}

//#####################################################################################################
void Scene::calcSelectionBoundingRect()
{
    d->m_selected_items_path = QPainterPath();
    foreach (AbstractItemInterface * item, d->m_selected_items.keys())
        d->m_selected_items_path = d->m_selected_items_path.united(item->mapToScene(item->shape()));
}

//#####################################################################################################
void Scene::rotateSelectedItems(const QPointF & rotationPoint, qreal angle)
{
    foreach (AbstractItemInterface * item, d->m_selected_items.keys())
    {
        QPointF point = rotationPoint;
        QTransform transform;
        transform.translate(point.rx(), point.ry());
        transform.rotate(angle);
        transform.translate(-point.rx(), -point.ry());
        QRectF updateRect = item->mapRectToScene(item->boundingRect());
        QTransform rotated = item->transform() * transform;
        item->setTransform(rotated);
        updateRect = updateRect.united( item->mapRectToScene( item->boundingRect() ) );
        if (item->scene())
            item->scene()->invalidate(updateRect);
    }
}

//#####################################################################################################
void Scene::rotationCommand(const QPointF & rotationPoint, qreal angle)
{
    QUndoCommand * command = new RotateItemsCommand(d->m_selected_items.keys(),
                                                    rotationPoint,
                                                    angle);
    PFE_PostUndoCommand(command);
}

//#####################################################################################################
bool Scene::askAboutRemoving(int count)
{
    if (count)
    {
        int result = KMessageBox::questionYesNo(KApplication::activeWindow(), i18n("Are you sure you want to delete %1 selected item%2?", QString::number(count), (count>1?"s":"")), i18n("Items deleting"));
        if (result == KMessageBox::Yes)
            return true;
    }
    return false;
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
