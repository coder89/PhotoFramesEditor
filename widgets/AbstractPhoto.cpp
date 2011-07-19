#include "AbstractPhoto.h"
#include "Scene.h"
#include "photo_context_menu.h"
#include "PhotoEffectsGroup.h"

#include <QMenu>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainterPath>
#include <QPolygonF>
#include <QPainter>
#include <QVariant>
#include <qmath.h>

#include <QDebug>
#include <QTime>

using namespace KIPIPhotoFramesEditor;

const QColor AbstractPhoto::SELECTED_ITEM_COLOR(255,0,0,20);

AbstractPhoto::AbstractPhoto(QGraphicsScene * scene) :
    QGraphicsPixmapItem(0,scene),
    m_name("New layer"),
    m_pixmap_original(),
    m_border_width(0)
{
    //this->setAcceptDrops(true);
    this->setAcceptHoverEvents(true);

    // Effects group
    m_effects_group = new PhotoEffectsGroup(this);
}

AbstractPhoto::~AbstractPhoto()
{
    m_effects_group->deleteLater();
}

void AbstractPhoto::setupWidget(const QPainterPath & path)
{
    this->m_image_path = path.translated(-path.boundingRect().topLeft());
    this->m_image_path = m_image_path.simplified();
    m_pixmap_original = QPixmap(m_image_path.boundingRect().size().toSize());
    m_pixmap_original.fill(Qt::transparent);
    this->recalcShape();

    // Create effective pixmap
    this->refreshPixmap();

    this->setFlag(QGraphicsItem::ItemIsSelectable);

    // Default border style
    this->setBorderWidth(0);
    this->setBorderColor(Qt::black);
    this->setBorderCornersStyle(Qt::RoundJoin);
}

void AbstractPhoto::setupWidget(const QPixmap & photo)
{
    m_pixmap_original = photo;

    // Scaling if to big
    QSize s = photo.size();
    QRect r = photo.rect();
    QSize sceneSize = scene()->sceneRect().size().toSize();
    if (sceneSize.width()<s.width() || sceneSize.height()<s.height())
    {
        s.scale(sceneSize*0.8, Qt::KeepAspectRatio);
        r.setSize(s);
    }

    QPainterPath p;
    p.addRect(r);
    m_image_path = p;
    this->m_image_path = m_image_path.simplified();
    this->recalcShape();

    // Create effective pixmap
    this->refreshPixmap();

    this->setFlag(QGraphicsItem::ItemIsSelectable);

    // Default border style
    this->setBorderWidth(0);
    this->setBorderColor(Qt::black);
    this->setBorderCornersStyle(Qt::RoundJoin);
}

void AbstractPhoto::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem unselectedOption = (*option);
    unselectedOption.state &= !QStyle::State_Selected;
    QGraphicsPixmapItem::paint(painter,&unselectedOption,widget);

//    // Selecting item
//    if ((option->state) & QStyle::State_Selected)
//    {
//        QPainterPath exposedPath = QPainterPath();
//        exposedPath.addRect(option->exposedRect);
//        painter->fillPath(m_complete_path.intersected(exposedPath), SELECTED_ITEM_COLOR);
//    }

    if (this->borderWidth())
        painter->fillPath(m_border_path, this->borderColor());
}

void AbstractPhoto::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dragEnterEvent";
    event->accept();
}

void AbstractPhoto::dragLeaveEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dragLeaveEvent";
    event->accept();
}

void AbstractPhoto::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dragMoveEvent";
    event->accept();
}

void AbstractPhoto::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "dropEvent";
    event->accept();
}

void AbstractPhoto::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsPixmapItem::mouseMoveEvent(event);
    if (QGraphicsItem::flags() & ItemIsMovable)
    {
        if (event->modifiers() & Qt::ShiftModifier)
        {
            QPointF p;
            event->accept();
            p = event->scenePos() - event->buttonDownPos(Qt::LeftButton);
            p.setX(x_grid*round(p.rx()/x_grid));
            p.setY(y_grid*round(p.ry()/y_grid));
            this->setPos(p.rx(),p.ry());
        }
        else
            QGraphicsPixmapItem::mouseMoveEvent(event);
//            p = this->pos() + event->pos() - event->lastPos();
    }
}

void AbstractPhoto::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsPixmapItem::mousePressEvent(event);
    if (this->isSelected())
        this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void AbstractPhoto::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsPixmapItem::mouseReleaseEvent(event);
    if (this->isSelected())
        this->setCursor(QCursor(Qt::OpenHandCursor));
}

void AbstractPhoto::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsPixmapItem::hoverEnterEvent(event);
    if (this->isSelected())
        this->setCursor(QCursor(Qt::OpenHandCursor));
}

void AbstractPhoto::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    QGraphicsPixmapItem::hoverLeaveEvent(event);
    this->unsetCursor();
}

void AbstractPhoto::updateIcon()
{
    m_icon = QIcon(m_pixmap.scaled(100,100,Qt::KeepAspectRatioByExpanding));
}

void AbstractPhoto::selectStateChanged(bool state)
{
    this->setSelected(state);
}

void AbstractPhoto::showStateChanged(bool state)
{
    this->setVisible(state);
}

void AbstractPhoto::lockStateChanged(bool state)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, state);
}

void AbstractPhoto::recalcShape()
{
    if (m_border_width)
    {
        QPainterPathStroker s;
        s.setWidth(m_border_width);
        s.setJoinStyle(m_border_corner_style);
        m_border_path = s.createStroke(m_image_path);
    }
    else
        m_border_path = QPainterPath();
    m_complete_path = m_border_path.united(m_image_path);
    prepareGeometryChange();

    try // I'm not sure how to properly/safely cast from QGraphicsScene* -> Scene*
    {
        Scene * m_scene = (Scene*)this->scene();
        if (m_scene)
            m_scene->updateSelection();
    }
    catch(...)
    {}
}

void AbstractPhoto::refreshPixmap()
{
    if (m_pixmap_original.isNull())
        return;
    this->m_pixmap = m_effects_group->apply(m_pixmap_original.scaled(this->boundingRect().size().toSize(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    this->updateIcon();
    QGraphicsPixmapItem::setPixmap(m_pixmap);
}
