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
        foreach (AbstractPhoto * photo, m_selected_items)
        {
            photo->setSelected(false);
            if (photo->hasFocus())
                photo->clearFocus();
        }
        m_selected_items.clear();
        m_selected_items_path = QPainterPath();
        setSelectionInitialPosition();
    }
    bool selectPressed()
    {
        if (m_pressed_item)
        {
            if (!m_pressed_item->isSelected())
            {
                m_selected_items.append(m_pressed_item);
                m_selected_items_path = m_selected_items_path.united(m_pressed_item->mapToScene(m_pressed_item->shape()));
                m_selected_items_all_movable &= m_pressed_item->flags() & QGraphicsItem::ItemIsMovable;
                m_pressed_item->setSelected(true);
                setSelectionInitialPosition();
            }
            return true;
        }
        return false;
    }
    void setSelectionInitialPosition()
    {
        if (m_selected_items_path.isEmpty())
            m_selected_items_path_initial_pos = QPointF();
        m_selected_items_path_initial_pos = m_selected_items_path.boundingRect().topLeft();
    }
    QList<AbstractPhoto*> m_selected_items;
    AbstractPhoto * m_pressed_item;
    QPainterPath m_selected_items_path;
    QPointF m_selected_items_path_initial_pos;
    bool m_selected_items_all_movable;
    bool m_selection_visible;

    int m_mode;
    friend class Scene;
};

class KIPIPhotoFramesEditor::MoveItemsUndoCommand : public QUndoCommand
{
        QList<AbstractPhoto*> m_items;
        QPointF m_movement;
        Scene * m_scene;
        bool done;
    public:
        MoveItemsUndoCommand(QList<AbstractPhoto*> items, QPointF movement, Scene * scene, QUndoCommand * parent = 0) :
            QUndoCommand(parent),
            m_items(items),
            m_movement(movement),
            m_scene(scene),
            done(true)
        {}
        virtual void redo();
        virtual void undo();
};
void MoveItemsUndoCommand::redo()
{
    if (!done)
    {
        foreach (AbstractPhoto * item, m_items)
            item->setPos(item->pos() + m_movement);
        done = !done;
        m_scene->calcSelectionBoundingRect();
    }
}
void MoveItemsUndoCommand::undo()
{
    if (done)
    {
        foreach (AbstractPhoto * item, m_items)
            item->setPos(item->pos() - m_movement);
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
    setMode(DEFAULT_EDITING_MODE);

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
    this->editingMode = Drawing;
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
    editingMode = DEFAULT_EDITING_MODE;
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
    if (d->m_selected_items.count() == 1 && (d->m_selected_items.at(0)->hasFocus()))
    {
        this->setFocusItem(d->m_selected_items.at(0));
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
        // Get initial selection position
        d->setSelectionInitialPosition();

        // If single selection mode, clear CTRL modifier
        if (selectionMode & SingleSelection)
            event->setModifiers(event->modifiers() & !Qt::ControlModifier);
        // Check if selected
//        if (d->m_selected_items_path.contains(event->scenePos()))
//        {
//            qDebug() << "contains:" << d->m_selected_items_path.contains(event->scenePos()) << event->scenePos();
//            d->
//            event->setAccepted(true);
//            return;
//        }
        // If not selected, then save item for further selection
        //else
        {
            d->m_pressed_item = dynamic_cast<AbstractPhoto*>(this->itemAt(event->scenePos()));
            if (!(event->modifiers() & Qt::ControlModifier) && !d->m_selected_items_path.contains(event->scenePos()))
                d->deselectSelected();
            if (d->m_pressed_item)
            {
                // If not selectable -> deselect item
                if (!(d->m_pressed_item->flags() & QGraphicsItem::ItemIsSelectable))
                    d->m_pressed_item = 0;
                else
                {
                    // If is selectable and focusable try to set focus and post mousepressevent to it
                    d->m_pressed_item->setFocus(Qt::MouseFocusReason);
                    if (d->m_pressed_item->hasFocus())
                    {
                        event->setPos(d->m_pressed_item->mapFromScene(event->scenePos()));
                        event->setButtonDownPos(event->button(),
                                                d->m_pressed_item->mapFromScene(event->buttonDownScenePos(event->button())));
                        event->setLastPos(d->m_pressed_item->mapFromScene(event->lastScenePos()));
                        d->m_pressed_item->mousePressEvent(event);
                    }
                }
            }
            event->setAccepted(d->m_pressed_item);
        }
    }
    else
        QGraphicsScene::mousePressEvent(event);
}

//#####################################################################################################
void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton)
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
            d->m_selected_items_path.translate(-difference);
            difference = distance - difference;
            d->m_selected_items_path.translate(distance);
            foreach (AbstractPhoto * item, d->m_selected_items)
                item->setPos(item->pos() + difference);
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
        // Selecting pressed item
        event->setAccepted(d->selectPressed());

        // Post move command to QUndoStack
        if (d->m_selected_items_path.boundingRect().topLeft() != d->m_selected_items_path_initial_pos)
        {
            QPointF movement = d->m_selected_items_path.boundingRect().topLeft()-d->m_selected_items_path_initial_pos;
            QUndoCommand * command = new MoveItemsUndoCommand(d->m_selected_items, movement, this);
            PFE_PostUndoCommand(command);
        }
    }
    else
        QGraphicsScene::mouseReleaseEvent(event);
}

// #####################################################################################################
void Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        switch(this->editingMode)
        {
            case Moving:
                QGraphicsScene::mouseDoubleClickEvent(event);
                break;
            case Drawing:
//                {
//                    temp_path.closeSubpath();
//                    PolygonWidget * tempItem = new PolygonWidget(temp_path, this);
//                    this->addItem(tempItem);
//                    QPointF p = temp_path.boundingRect().topLeft();
//                    tempItem->moveBy(p.rx(),p.ry());
//                    disableitemsDrawing();
//                }
                break;
            default:
                goto    others;
        }
        event->accept();
    }
    else
    {
        others:
            QGraphicsScene::mouseDoubleClickEvent(event);
    }
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
        painter->setCompositionMode(QPainter::CompositionMode_Exclusion);
        painter->save();
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
void Scene::setMode(EditMode mode)
{
    switch(mode)
    {
        case Rotating:
            editingMode = mode;
            setSelectionMode(SingleSelection);
            d->setMode(ScenePrivate::Rotation);
            break;
        case BorderEdit:
            editingMode = mode;
            setSelectionMode(SingleSelection);
            break;
        case Moving:
        default:
            setSelectionMode(MultiSelection);
            editingMode = DEFAULT_EDITING_MODE;
    }
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
            result.appendChild(photo->toSvg(document, true)); /// TODO : true (should be able to set to false!)
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
                           i18n("I was unable to read %d elements!\nInvalid or corrupted image file!", errorsCount));
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
    d->m_selected_items.clear();
    d->m_selected_items_path = QPainterPath();
    QList<AbstractPhoto*> itemsList = this->selectedItems();
    foreach (AbstractPhoto * item, itemsList)
    {
        if (d->m_selected_items.contains(item))
            continue;
        d->m_selected_items.append(item);
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
