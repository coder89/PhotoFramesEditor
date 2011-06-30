#include "QGraphicsSelectionItem.h"

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::RotationPoint : public QGraphicsWidget
{
        QPainterPath path;
    public:
        RotationPoint(QGraphicsItem * parent):
            QGraphicsWidget(parent)
        {
            path.addEllipse(0,0,20,20);
            this->setAcceptHoverEvents(true);
            this->setFlag(QGraphicsItem::ItemIsMovable, true);
        }

        virtual void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
        {
            painter->drawEllipse(0,0,20,20);
            painter->setBrush(Qt::black);
            painter->drawEllipse(7,7,6,6);
        }

        virtual QPainterPath shape() const
        {
            return path;
        }

        virtual QPainterPath opaqueArea() const
        {
            return path;
        }

        virtual QRectF boundingRect() const
        {
            return path.boundingRect();
        }

    protected:

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event)
        {
            this->setCursor(QCursor(Qt::BusyCursor));
        }

        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event)
        {
            qDebug() << "PressPoinrBefore:" << event->pos() << event->lastPos();
            QGraphicsWidget::mousePressEvent(event);
            qDebug() << "PressPoinrAfter:" << event->pos() << event->lastPos();
        }

        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event)
        {
            qDebug() << "MovePoint:" << event->pos() << event->buttonDownPos(Qt::LeftButton);
            QPointF p = mapToParent(event->pos()-event->buttonDownPos(Qt::LeftButton));
            setPos(p.rx(),p.ry());
            event->accept();
        }
};

QGraphicsSelectionItem::QGraphicsSelectionItem(QGraphicsItem * parent) :
    QGraphicsWidget(parent),
    m_rot_point(new RotationPoint(this))
{
    this->setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);
    //this->setHandlesChildEvents(false);

    // Rotation point
    m_rot_point->setParent(this);
    setRotationVisible(false);
}

void QGraphicsSelectionItem::setRotationVisible(bool visible)
{
    m_rot_point->setVisible(visible);
}

void QGraphicsSelectionItem::setSelection(const QList<QGraphicsItem*> & items)
{
    QList<QGraphicsItem*> tempItems = items;
    tempItems.removeAll(m_rot_point);
    QSet<QGraphicsItem*> unselected = m_itemsList - tempItems.toSet();
    foreach (QGraphicsItem * item, unselected)
        this->removeItem(item);
    QSet<QGraphicsItem*> selected = tempItems.toSet() - m_itemsList;
    foreach (QGraphicsItem * item, selected)
        this->addItem(item);
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

void QGraphicsSelectionItem::addToShape(const QPainterPath & path)
{
    m_shape.translate(pos());
    m_shape = m_shape.united(path);
    setPos(m_shape.boundingRect().topLeft());
    resize(m_shape.boundingRect().size());
    m_shape.translate(-pos());
    QRectF tempRect = m_shape.boundingRect();
    if (tempRect.width() && tempRect.height())
    {
        m_rot_point->setPos(m_shape.boundingRect().center()-QPointF(10,10));
        m_rot_point->setVisible(true);
    }
    else
        m_rot_point->setVisible(false);
}

void QGraphicsSelectionItem::removeFromShape(const QPainterPath & path)
{
    m_shape.translate(pos());
    m_shape = m_shape.subtracted(path);
    setPos(m_shape.boundingRect().topLeft());
    resize(m_shape.boundingRect().size());
    m_shape.translate(-pos());
    QRectF tempRect = m_shape.boundingRect();
    if (tempRect.width() && tempRect.height())
    {
        m_rot_point->setPos(m_shape.boundingRect().center()-QPointF(10,10));
        m_rot_point->setVisible(true);
    }
    else
        m_rot_point->setVisible(false);
}

QRectF QGraphicsSelectionItem::boundingRect() const
{
    QPainterPath temp = m_shape;
     if (m_rot_point->isVisible())
         temp += m_rot_point->shape().translated(m_rot_point->pos());
    return temp.boundingRect();
}

bool QGraphicsSelectionItem::contains(const QPointF & point) const
{
    QPainterPath temp = m_shape;
     if (m_rot_point->isVisible())
         temp += m_rot_point->opaqueArea().translated(m_rot_point->pos());
    return temp.contains(point);
}

QPainterPath QGraphicsSelectionItem::opaqueArea() const
{
    QPainterPath temp = m_shape;
    if (m_rot_point->isVisible())
        temp += m_rot_point->opaqueArea().translated(m_rot_point->pos());
    return temp;
}

QPainterPath QGraphicsSelectionItem::shape() const
{
    QPainterPath temp = m_shape;
    if (m_rot_point->isVisible())
        temp += m_rot_point->shape().translated(m_rot_point->pos());
    return temp;
}
