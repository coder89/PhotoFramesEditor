#include "QGraphicsRotationItem.h"

using namespace KIPIPhotoFramesEditor;

RotationHandler::RotationHandler(QGraphicsItem * parent) :
    AbstractItemInterface(parent)
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    m_path.addRect(0,0,10,10);
    reset();
}

void RotationHandler::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->drawPath(m_path);
}

QPainterPath RotationHandler::shape() const
{
    return m_path;
}

QPainterPath RotationHandler::opaqueArea() const
{
    return m_path;
}

QRectF RotationHandler::boundingRect() const
{
    return m_path.boundingRect();
}

void RotationHandler::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    this->setCursor(QCursor(Qt::OpenHandCursor));
}

void RotationHandler::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    this->unsetCursor();
}

void RotationHandler::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "handler: press";
    m_initial_position = pos();
    this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void RotationHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "handler: release";
    this->setCursor(QCursor(Qt::OpenHandCursor));
}

void RotationHandler::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "handler: move";
    QGraphicsView *view = 0;
    if (event->widget())
        view = qobject_cast<QGraphicsView *>(event->widget()->parentWidget());
    QTransform itemTransform;
    itemTransform = this->transform();
    QTransform viewToParentTransform = itemTransform * sceneTransform().inverted();
    QPointF currentParentPos;
    QPointF lastParentPos;
    QPointF buttonDownParentPos;
    currentParentPos = viewToParentTransform.map(QPointF(view->mapFromGlobal(event->screenPos())));
    lastParentPos = viewToParentTransform.map(QPointF(view->mapFromGlobal(event->lastScreenPos())));
    buttonDownParentPos = viewToParentTransform.map(QPointF(view->mapFromGlobal(event->buttonDownScreenPos(Qt::LeftButton))));
    QPointF currentPos = m_initial_position+currentParentPos-buttonDownParentPos;
    QPointF lastPos = pos();
    qreal currentLength = qSqrt(currentPos.rx()*currentPos.rx()+currentPos.ry()*currentPos.ry());
    qreal lastLength = qSqrt(lastPos.rx()*lastPos.rx()+lastPos.ry()*lastPos.ry());
    qreal scale = lastLength/currentLength;
    qreal scalar = currentPos.rx()*lastPos.rx()+currentPos.ry()*lastPos.ry();
    qreal cos = scalar/(currentLength*lastLength);
    if (currentPos.rx()*lastPos.ry()-currentPos.ry()*lastPos.rx()>0)
        m_rotation_angle = -180*qAcos(cos)/M_PI;
    else
        m_rotation_angle = 180*qAcos(cos)/M_PI;
    setPos(currentPos.rx()*scale,currentPos.ry()*scale);
    emit rotationAngleChanged(m_rotation_angle,event->modifiers()&Qt::ShiftModifier);
}




QGraphicsRotationItem::QGraphicsRotationItem(QGraphicsItem * parent):
    AbstractItemInterface(parent)
    //m_handler(new RotationHandler(this))
{
    //m_handler->setParent(this);
    //m_handler->setPos(100,5);
    //connect(m_handler,SIGNAL(rotationAngleChanged(qreal,bool)),this,SLOT(emitRotationChanges(qreal,bool)));
    m_handler_path.addRect(0,0,10,10);

    m_elipse_path.addEllipse(0,0,20,20);
    m_path += m_elipse_path;
    m_path.moveTo(10,10);
    m_path.lineTo(100,10);

    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);

    reset();
}

void QGraphicsRotationItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
    painter->drawEllipse(0,0,20,20);
    painter->setBrush(Qt::black);
    painter->drawEllipse(7,7,6,6);
    painter->drawPath(m_handler_path.translated(m_handler_pos));
   // painter->drawLine(QPoint(10,10),m_handler->pos()+m_handler->boundingRect().center());
}

QPainterPath QGraphicsRotationItem::shape() const
{
    return m_path + m_handler_path.translated(m_handler_pos);
}

QPainterPath QGraphicsRotationItem::opaqueArea() const
{
    return shape();
}

QRectF QGraphicsRotationItem::boundingRect() const
{
    return shape().boundingRect();
}

void QGraphicsRotationItem::center(const QRectF & boundingRect)
{
    qDebug() << boundingRect;
    this->setPos(boundingRect.center() - QPointF(10,10));
}

void QGraphicsRotationItem::hoverEnterEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    this->setCursor(QCursor(Qt::OpenHandCursor));
}

void QGraphicsRotationItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    this->unsetCursor();
}

void QGraphicsRotationItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "rot: press" << event->buttonDownPos(Qt::LeftButton) << m_elipse_path.contains(event->buttonDownPos(Qt::LeftButton));
    if (m_elipse_path.contains(event->buttonDownPos(Qt::LeftButton)))
    {
        m_elipse_pressed = true;
        m_initial_position = pos();
    }
    else
        m_elipse_pressed = false;
    this->setCursor(QCursor(Qt::ClosedHandCursor));
}

void QGraphicsRotationItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    this->setCursor(QCursor(Qt::OpenHandCursor));
}

void QGraphicsRotationItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "rot: move" << event->buttonDownScenePos(Qt::LeftButton) << m_elipse_path.contains(event->buttonDownPos(Qt::LeftButton));
    if (m_elipse_pressed)
    {
//        QGraphicsView *view = 0;
//        if (event->widget())
//            view = qobject_cast<QGraphicsView *>(event->widget()->parentWidget());
//        QTransform itemTransform;
//        itemTransform = this->transform();
//        QTransform viewToParentTransform = itemTransform * sceneTransform().inverted();
//        QPointF currentParentPos;
//        QPointF lastParentPos;
//        QPointF buttonDownParentPos;
//        currentParentPos = viewToParentTransform.map(QPointF(view->mapFromGlobal(event->screenPos())));
//        lastParentPos = viewToParentTransform.map(QPointF(view->mapFromGlobal(event->lastScreenPos())));
//        buttonDownParentPos = viewToParentTransform.map(QPointF(view->mapFromGlobal(event->buttonDownScreenPos(Qt::LeftButton))));
//        QPointF currentPos = m_initial_position+currentParentPos-buttonDownParentPos;
//        m_rot_point += currentPos-pos();
//        setPos(currentPos.rx(),currentPos.ry());
        QPointF dif = event->scenePos() - event->lastScenePos();
        moveBy(dif.x(), dif.y());
        emit rotationPointChanged(m_rot_point);
    }
    else
    {
//        QPointF currentPos = m_handler_pos;
//        qreal currentLength = qSqrt(currentPos.rx()*currentPos.rx()+currentPos.ry()*currentPos.ry());
//        qreal lastLength = qSqrt(lastPos.rx()*lastPos.rx()+lastPos.ry()*lastPos.ry());
//        qreal scale = lastLength/currentLength;
//        qreal scalar = currentPos.rx()*lastPos.rx()+currentPos.ry()*lastPos.ry();
//        qreal cos = scalar/(currentLength*lastLength);
//        qreal m_rotation_angle;
//        if (currentPos.rx()*lastPos.ry()-currentPos.ry()*lastPos.rx()>0)
//            m_rotation_angle = -180*qAcos(cos)/M_PI;
//        else
//            m_rotation_angle = 180*qAcos(cos)/M_PI;
//        m_handler_pos = QPointF(currentPos.rx()*scale,currentPos.ry()*scale);
//        qDebug() << m_handler_pos;
    }
    event->accept();
}

void QGraphicsRotationItem::emitRotationChanges(qreal deg, bool round)
{
    this->update();
    emit rotationAngleChanged(deg, round);
}
