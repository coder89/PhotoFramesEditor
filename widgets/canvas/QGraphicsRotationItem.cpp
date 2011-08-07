#include "QGraphicsRotationItem.h"

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::QGraphicsRotationItemPrivate
{
    QGraphicsRotationItemPrivate(QGraphicsRotationItem * item) :
        item(item)
    {
        handler_path.addRect(-5,-5,10,10);
        elipse_path.addEllipse(-10,-10,20,20);

        path += elipse_path;
        path.moveTo(0,0);
        path.lineTo(100,0);
    }
    QGraphicsRotationItem * item;

    QPointF viewportToItemPosition(const QPoint & pos, QWidget * widget)
    {
        QGraphicsView *view = 0;
        if (widget)
            view = qobject_cast<QGraphicsView *>(widget->parentWidget());

        if (view)
        {
            QTransform deviceTransform = item->deviceTransform(view->viewportTransform()).inverted();
            return deviceTransform.map(QPointF(view->mapFromGlobal(pos)));
        }

        return pos;
    }
    QRectF itemToViewportRect(const QRectF & rect, QWidget * widget)
    {
        QGraphicsView *view = 0;
        if (widget)
            view = qobject_cast<QGraphicsView *>(widget->parentWidget());

        if (view)
            return item->mapRectToScene(view->transform().inverted().mapRect(rect));

        return rect;
    }
    QPainterPath rotated_shape;
    QPointF rotation_point;
    QPointF rotation_point_offset;
    QPainterPath path;
    QPainterPath elipse_path;
    QPainterPath handler_path;
    QPointF initial_position;
    QPointF handler_pos;
    qreal rotation_angle;
    bool elipse_pressed;

    friend class QGraphicsRotationItem;
};

QGraphicsRotationItem::QGraphicsRotationItem(QGraphicsItem * parent):
    AbstractItemInterface(parent),
    d(new QGraphicsRotationItemPrivate(this))
{
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable, false);
    reset();
}

void QGraphicsRotationItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
    painter->save();
    painter->setPen(QPen(Qt::red, 2));
    painter->setCompositionMode(QPainter::RasterOp_NotSourceAndNotDestination);
    painter->drawPath((d->elipse_path));
    painter->setBrush(Qt::white);
    painter->drawEllipse(-3,-3,6,6);
    painter->restore();
}

QPainterPath QGraphicsRotationItem::shape() const
{
    return d->path + d->rotated_shape;
}

QPainterPath QGraphicsRotationItem::opaqueArea() const
{
    return shape();
}

QRectF QGraphicsRotationItem::boundingRect() const
{
    return shape().boundingRect();
}

void QGraphicsRotationItem::initRotation(const QPainterPath & path, const QPointF & rotationPoint)
{
    d->rotated_shape = path;
    QRectF boundingRect = path.boundingRect();
    this->setPos(boundingRect.center());
    d->rotated_shape.translate(-boundingRect.center());
    d->handler_pos = QPointF(100,0);
    d->rotation_point = rotationPoint;
    d->rotation_angle = 0;
    d->rotation_point_offset = QPointF();
}

void QGraphicsRotationItem::reset()
{
    d->rotated_shape = QPainterPath();
    d->handler_pos = QPointF(100,0);
    d->rotation_point = QPointF();
    d->rotation_point_offset = QPointF();
    d->rotation_angle = 0;
}

qreal QGraphicsRotationItem::angle() const
{
    return d->rotation_angle;
}

QPointF QGraphicsRotationItem::rotationPoint() const
{
    return d->rotation_point + d->rotation_point_offset;
}

bool QGraphicsRotationItem::isRotated() const
{
    return d->rotation_angle != 0;
}

void QGraphicsRotationItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    qDebug() << "rot enter";
    this->setCursor(QCursor(Qt::OpenHandCursor));
}

void QGraphicsRotationItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    qDebug() << "rot leave";
    this->unsetCursor();
}

void QGraphicsRotationItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QPointF buttonDownPos = d->viewportToItemPosition(event->buttonDownScreenPos(Qt::LeftButton), event->widget());
    QRectF buttonDownPosRect(buttonDownPos-QPointF(1,1), QSizeF(2,2));
    if (d->elipse_path.intersects(buttonDownPosRect))
    {
        d->elipse_pressed = true;
        d->initial_position = pos();
    }
    else
        d->elipse_pressed = false;
    this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void QGraphicsRotationItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * /*event*/)
{
    this->setCursor(QCursor(Qt::OpenHandCursor));
    if (d->rotation_angle)
        emit rotationFinished(d->rotation_point+d->rotation_point_offset, d->rotation_angle);
    d->rotation_angle = 0;
}

void QGraphicsRotationItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (d->elipse_pressed)
    {
        QPointF dif = event->scenePos() - event->lastScenePos();
        moveBy(dif.x(), dif.y());
        d->rotation_point_offset += dif;
        d->rotated_shape.translate(-dif);
    }
    else
    {
        // Calculate movement parameters
        QRectF refreshRect = this->boundingRect();
        QPointF itemPos = d->viewportToItemPosition(event->screenPos(), event->widget());
        QPointF currentPos = d->viewportToItemPosition(event->buttonDownScreenPos(Qt::LeftButton), event->widget());
        qreal currentLength = qSqrt(currentPos.rx()*currentPos.rx()+currentPos.ry()*currentPos.ry());
        qreal newLength = qSqrt(itemPos.rx()*itemPos.rx()+itemPos.ry()*itemPos.ry());
        qreal scalar = currentPos.rx()*itemPos.rx()+currentPos.ry()*itemPos.ry();

        // Calculate angle
        qreal cos = scalar/(currentLength*newLength);
        qreal prev_rotation_angle = d->rotation_angle;
        if (currentPos.rx()*itemPos.ry()-currentPos.ry()*itemPos.rx()>0)
            d->rotation_angle = 180*qAcos(cos)/M_PI;
        else
            d->rotation_angle = -180*qAcos(cos)/M_PI;

        // Rotation with 15 deegres step - Shift modifier
        if (event->modifiers() & Qt::ShiftModifier)
            d->rotation_angle = qRound(d->rotation_angle / 15) * 15.0;
        if (d->rotation_angle == prev_rotation_angle)
            return;

        QTransform transform;
        transform.rotate(d->rotation_angle-prev_rotation_angle);
        d->rotated_shape = transform.map(d->rotated_shape);

        // Updates widgets view
        refreshRect = refreshRect.united(this->boundingRect());
        this->scene()->invalidate(d->itemToViewportRect(refreshRect, event->widget()));

        // Emits rotation signal
        emit rotationChanged(d->rotation_point+d->rotation_point_offset, d->rotation_angle-prev_rotation_angle);
    }
    event->accept();
}
