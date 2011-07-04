#include "QGraphicsSelectionItem.h"
#include "abstract_photo.h"

#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include <QSet>
#include <qmath.h>

using namespace KIPIPhotoFramesEditor;

QGraphicsSelectionItem::QGraphicsSelectionItem(QGraphicsItem * parent) :
    QGraphicsWidget(parent),
    m_shape()
{
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsMovable, false);
    this->setFlag(QGraphicsItem::ItemIsSelectable, false);
}

//void QGraphicsSelectionItem::setRotationVisible(bool visible)
//{
//    if (visible)
//        m_flags |= Rotation;
//    else
//        m_flags &= !Rotation;
//    m_rot_widget->setPos(m_shape.boundingRect().center()-QPointF(10,10));
//    m_rot_widget->setVisible(visible && m_shape.boundingRect().isValid());
//}

QPointF QGraphicsSelectionItem::setSelection(const QSet<QGraphicsItem*> & items)
{
    this->m_itemsList = items;
    return setupWidget();
}

void QGraphicsSelectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QPointF p = event->pos()-event->lastPos();
    foreach (QGraphicsItem * item, m_itemsList)
        item->moveBy(p.rx(), p.ry());
    event->setAccepted(true);
    qDebug() << "mouseMoveEvent: " << event->isAccepted();
}

void QGraphicsSelectionItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    setCursor(QCursor(Qt::ClosedHandCursor));
    QGraphicsWidget::mousePressEvent(event);
    qDebug() << "mousePessEvent: " << event->isAccepted();
}

void QGraphicsSelectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    unsetCursor();
    QGraphicsWidget::mouseReleaseEvent(event);
}

void QGraphicsSelectionItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    setCursor(QCursor(Qt::OpenHandCursor));
    QGraphicsWidget::hoverEnterEvent(event);
}

void QGraphicsSelectionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    unsetCursor();
    QGraphicsWidget::hoverLeaveEvent(event);
}

QRectF QGraphicsSelectionItem::boundingRect() const
{
    return shape().boundingRect();
}

bool QGraphicsSelectionItem::contains(const QPointF & point) const
{
    return shape().contains(point);
}

QPainterPath QGraphicsSelectionItem::opaqueArea() const
{
    return shape();
}

QPainterPath QGraphicsSelectionItem::shape() const
{
    QPainterPath result = m_shape;
    return result;
}

QPainterPath QGraphicsSelectionItem::calcShape() const
{
    QPainterPath result;
    foreach (QGraphicsItem * item, m_itemsList)
    {
        QPainterPath temp = item->shape();
        temp =  item->transform().map(temp);
        temp.translate(item->pos());
        result = result.united(temp);
    }
    return result;
}

QPointF QGraphicsSelectionItem::setupWidget()
{
    this->resetTransform();
    m_shape = calcShape();
    QPointF offset = m_shape.boundingRect().topLeft();
    m_shape.translate(-offset);
    resize(m_shape.boundingRect().size());
    return offset;
}

void QGraphicsSelectionItem::setRotation(qreal angle, const QPointF & rotPoint, bool round)
{
    QPointF point = (rotPoint);
    foreach (QGraphicsItem * item, m_itemsList)
    {
        QPointF temp = point;
        qreal x = temp.rx();
        qreal y = temp.ry();
        item->setTransform(item->transform()*(QTransform().translate(x,y).rotate(angle).translate(-x,-y)));
    }
    m_shape = this->calcShape();
    m_shape.translate(-this->scenePos());
}

