#include "Scene.h"
#include "QGraphicsEditionWidget.h"

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

// KDE
#include <kapplication.h>
#include <kmessagebox.h>
#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

namespace KIPIPhotoFramesEditor
{
    class ScenePrivate
    {
        enum
        {
            Rotation = 1,
            Moving = 2,
        };

        ScenePrivate(Scene * parent) :
            m_edit_widget(new QGraphicsEditionWidget),
            m_parent(parent)
        {
            // Editing widget
            setMode(0);
            QObject::connect(m_edit_widget, SIGNAL(deleteSelectedItems()), parent, SLOT(removeSelectedItems()));
            parent->QGraphicsScene::addItem(m_edit_widget);
            m_edit_widget->setZValue(1.0/0.0);

            // Background
            m_background = new QGraphicsRectItem(parent->sceneRect(), 0, parent);
            m_background->setZValue(-1.0/0.0);
            m_background->setBrush(Qt::white);
            m_background->setPen(QPen(Qt::transparent, 0));
        }

        void setMode(int mode)
        {
            m_mode = mode;
            m_edit_widget->setRotationVisible(mode & Rotation);
            m_edit_widget->reset();
        }

        // Edition widget
        QGraphicsEditionWidget * m_edit_widget;
        // Parent scene
        QGraphicsScene * m_parent;
        // Background item
        QGraphicsRectItem * m_background;

        int m_mode;

        friend class Scene;
    };
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
    switch(event->key())
    {
        case Qt::Key_Delete:
            this->removeItems(selectedItems());
            event->accept();
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

bool Scene::event(QEvent * event)
{
    switch(event->type())
    {
        default:
            return QGraphicsScene::event(event);
    }
}

//#####################################################################################################

void Scene::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        qDebug() << "isContains:" << event->scenePos() << d->m_edit_widget->contains(event->scenePos()-d->m_edit_widget->pos()) << d->m_edit_widget->shape();
        if (selectionMode & SingleSelection)
            event->setModifiers(event->modifiers() & !Qt::ControlModifier);
        if (d->m_edit_widget->contains(event->scenePos()-d->m_edit_widget->pos()) && (!(event->modifiers() & Qt::ControlModifier)))
            goto press_others;
        d->m_edit_widget->setVisible(false);
        QGraphicsItem * clickedItem = this->itemAt(event->scenePos());
        if (!clickedItem)
            goto press_others;
        if (!(event->modifiers() & Qt::ControlModifier))
        {
            this->setSelectionArea(QPainterPath());
            clickedItem->setSelected(!clickedItem->isSelected());
        }
        this->calcSelectionBoundingRect();
        d->m_edit_widget->setVisible(true);
        d->m_edit_widget->setSelected(true);
    }
    press_others:
        QGraphicsScene::mousePressEvent(event);
}

//#####################################################################################################

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
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

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        switch(this->editingMode)
        {
            case Drawing:
                {
                    if (temp_path.elementCount() == 0)
                        break;
                    QPainterPath path = temp_path;
                    path.lineTo(event->scenePos());
                    temp_widget->setPath(path);
                }
                break;
            default:
                goto    others;
        }
    }
    else
    {
        others:
            QGraphicsScene::mouseMoveEvent(event);
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
            result.appendChild(photo->toSvg(document));
    }
    return result;
}

//#####################################################################################################

void Scene::fromSvg(QDomElement & svgImage)
{
    if (svgImage.tagName() != "svg")
        return;
    // Clear scene
    delete d;
    this->clear();
    d = new ScenePrivate(this);

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
        if (itemClass == "PhotoItem")
        {
            PhotoItem * item = PhotoItem::fromSvg(element);
            if (item)
                this->addItem(item);
            else
                ++errorsCount;
        }
    }

    // Show error message
    if (errorsCount)
    {
        KMessageBox::error(0,
                           i18n("I was unable to read %d elements!\nInvalid or corrupted image file!", errorsCount));
    }
}

//#####################################################################################################

void Scene::updateSelection()
{
    d->m_edit_widget->refresh();
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
    d->m_edit_widget->setSelection(this->QGraphicsScene::selectedItems());
}
