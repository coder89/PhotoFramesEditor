#include "QGraphicsSelectionItem.h"
#include "QGraphicsSelectionItem_p.h"

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
    m_rot_widget(new RotationWidget(this)),
    m_shape()
{
    this->setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);

    connect(m_rot_widget,SIGNAL(rotationChanged(qreal,bool)),this,SLOT(setRotation(qreal,bool)));

    // Rotation point
    m_flags = Default;
    setRotationVisible(true);
}

void QGraphicsSelectionItem::setRotationVisible(bool visible)
{
    if (visible)
        m_flags |= Rotation;
    else
        m_flags &= !Rotation;
    qDebug() << shape().boundingRect().isValid() << shape();
    m_rot_widget->setPos(shape().boundingRect().center()-QPointF(10,10));
    m_rot_widget->setVisible(visible && shape().boundingRect().isValid());
}

void QGraphicsSelectionItem::setSelection(const QList<QGraphicsItem*> & items)
{
    this->m_itemsList = items.toSet();
    setupWidget();
    setRotationVisible(m_flags & Rotation);
    m_rot_widget->reset();
}

void QGraphicsSelectionItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    QMenu menu;
    if (m_rot_widget->contains(event->pos()-m_rot_widget->pos()))
    {
        QAction * center = menu.addAction("Center position");
        connect(center,SIGNAL(triggered()),this,SLOT(setRotationVisible()));
    }
    if (menu.actions().count())
    {
        menu.exec(event->screenPos());
        event->setAccepted(true);
    }
    else
        event->setAccepted(false);
}

void QGraphicsSelectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QPointF p = event->pos()-event->lastPos();
    moveBy(p.rx(),p.ry());
    foreach (QGraphicsItem * item, m_itemsList)
        item->moveBy(p.rx(), p.ry());
    event->accept();
}

void QGraphicsSelectionItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    setCursor(QCursor(Qt::ClosedHandCursor));
    QGraphicsWidget::mousePressEvent(event);
}

void QGraphicsSelectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    setCursor(QCursor(Qt::OpenHandCursor));
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
    return m_shape.boundingRect();
}

bool QGraphicsSelectionItem::contains(const QPointF & point) const
{
    return m_shape.contains(point);
}

QPainterPath QGraphicsSelectionItem::opaqueArea() const
{
    return m_shape;
}

QPainterPath QGraphicsSelectionItem::shape() const
{
    return m_shape;
}

void QGraphicsSelectionItem::setRotation(qreal angle, bool round)
{
    QPointF rotBase = m_rot_widget->rotationPoint();
    foreach (QGraphicsItem * item, m_itemsList)
    {
        QPointF rotPoint = rotBase;
        qreal x = rotPoint.rx();
        qreal y = rotPoint.ry();
        item->setTransform(item->transform()*QTransform().translate(x,y).rotate(angle).translate(-x,-y));
    }
    setupWidget();
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

void QGraphicsSelectionItem::setupWidget()
{
    QPainterPath last_m_shape = m_shape;
    //m_shape = calcShape();
    //QPointF offset = m_shape.boundingRect().topLeft();
    //setPos(offset);
    //m_shape.translate(-offset);
    //setRotationVisible(m_flags & Rotation);
}

//void QGraphicsSelectionItem::addToShape(const QPainterPath & path)
//{
//    m_shape.translate(pos());
//    m_shape = m_shape.united(path);
//    setPos(m_shape.boundingRect().topLeft());
//    resize(m_shape.boundingRect().size());
//    m_shape.translate(-pos());
//    QRectF tempRect = m_shape.boundingRect();
//    m_rot_widget->setPos(m_shape.boundingRect().center()-QPointF(10,10));
//    m_rot_widget->setVisible(tempRect.isValid());
//    m_rot_widget->reset();
//}

//void QGraphicsSelectionItem::removeFromShape(const QPainterPath & path)
//{
//    m_shape.translate(pos());
//    m_shape = m_shape.subtracted(path);
//    setPos(m_shape.boundingRect().topLeft());
//    resize(m_shape.boundingRect().size());
//    m_shape.translate(-pos());
//    QRectF tempRect = m_shape.boundingRect();
//    m_rot_widget->setPos(m_shape.boundingRect().center()-QPointF(10,10));
//    m_rot_widget->setVisible(tempRect.isValid());
//    m_rot_widget->reset();
//}
