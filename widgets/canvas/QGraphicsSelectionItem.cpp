#include "QGraphicsSelectionItem.h"

#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include <qmath.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::RotationWidget : public QGraphicsWidget
{
    QPainterPath m_path;
    QPainterPath m_elipse_path;

    class RotationHandler : public QGraphicsWidget
    {
            QPainterPath m_path;
            QPointF initialPosition;
        public:
            RotationHandler(QGraphicsItem * parent) :
                QGraphicsWidget(parent)
            {
                this->setFlag(QGraphicsItem::ItemIsMovable);
                setFlag(QGraphicsItem::ItemIgnoresTransformations);
                m_path.addRect(0,0,10,10);
            }
            virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
            {
                painter->drawPath(m_path);
            }
            virtual QPainterPath shape() const
            {
                return m_path;
            }
            virtual QPainterPath opaqueArea() const
            {
                return m_path;
            }
            virtual QRectF boundingRect() const
            {
                return m_path.boundingRect();
            }
        protected:
            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event)
            {
                this->setCursor(QCursor(Qt::OpenHandCursor));
            }
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
            {
                this->unsetCursor();
            }
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event)
            {
                initialPosition = pos();
                this->setCursor(QCursor(Qt::ClosedHandCursor));
                QGraphicsWidget::mousePressEvent(event);
            }
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
            {
                this->setCursor(QCursor(Qt::OpenHandCursor));
                QGraphicsWidget::mouseReleaseEvent(event);
            }
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event)
            {
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
                QPointF currentPos = initialPosition+currentParentPos-buttonDownParentPos;
                QPointF lastPos = initialPosition;
                qreal currentLength = qSqrt(currentPos.rx()*currentPos.rx()+currentPos.ry()*currentPos.ry());
                qreal lastLength = qSqrt(lastPos.rx()*lastPos.rx()+lastPos.ry()*lastPos.ry());
                qreal scalar = currentPos.rx()*lastPos.rx()+currentPos.ry()*lastPos.ry();
                qreal cos = scalar/(currentLength*lastLength);
                qDebug() << cos;
                //parentItem()->setRotation(qAcos(cos)+parentItem()->rotation());
            }
    };

    RotationHandler * m_handler;

public:
    RotationWidget(QGraphicsItem * parent):
        QGraphicsWidget(parent),
        m_handler(new RotationHandler(this))
    {
        m_handler->setParent(this);
        m_handler->setPos(100,5);

        m_elipse_path.addEllipse(0,0,20,20);
        m_path += m_elipse_path;
        m_path.moveTo(10,10);
        m_path.lineTo(100,10);

        this->setAcceptHoverEvents(true);
        this->setHandlesChildEvents(false);
        this->setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIgnoresTransformations);
    }
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
    {
        painter->drawEllipse(0,0,20,20);
        painter->setBrush(Qt::black);
        painter->drawEllipse(7,7,6,6);
        painter->drawLine(QPoint(10,10),QPoint(100,10));
    }
    virtual QPainterPath shape() const
    {
        return m_path + m_handler->shape().translated(m_handler->pos());
    }
    virtual QPainterPath opaqueArea() const
    {
        return m_path + m_handler->shape().translated(m_handler->pos());
    }
    virtual QRectF boundingRect() const
    {
        return (m_path + m_handler->shape().translated(m_handler->pos())).boundingRect();
    }

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event)
    {
        this->setCursor(QCursor(Qt::OpenHandCursor));
    }
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
    {
        this->unsetCursor();
    }
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event)
    {
        this->setCursor(QCursor(Qt::ClosedHandCursor));
        QGraphicsWidget::mousePressEvent(event);
    }
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
    {
        this->setCursor(QCursor(Qt::OpenHandCursor));
        QGraphicsWidget::mouseReleaseEvent(event);
    }
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event)
    {
        if (m_elipse_path.contains(event->buttonDownPos(Qt::LeftButton)))
        {
            QGraphicsItem::mouseMoveEvent(event);
            return;
        }
        else if (m_handler->contains(event->buttonDownPos(Qt::LeftButton)))
        {
            QGraphicsItem::mouseMoveEvent(event);
            return;
        }
        event->accept();
    }
};

QGraphicsSelectionItem::QGraphicsSelectionItem(QGraphicsItem * parent) :
    QGraphicsWidget(parent),
    m_rot_widget(new RotationWidget(this))
{
    this->setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);

    // Rotation point
    setRotationVisible(false);
}

void QGraphicsSelectionItem::setRotationVisible(bool visible)
{
    m_rot_widget->setPos(m_shape.boundingRect().center()-QPointF(10,10));
    m_rot_widget->setVisible(visible && m_shape.boundingRect().isValid());
}

void QGraphicsSelectionItem::setSelection(const QList<QGraphicsItem*> & items)
{
    QList<QGraphicsItem*> tempItems = items;
    tempItems.removeAll(m_rot_widget);
    QSet<QGraphicsItem*> unselected = m_itemsList - tempItems.toSet();
    foreach (QGraphicsItem * item, unselected)
        this->removeItem(item);
    QSet<QGraphicsItem*> selected = tempItems.toSet() - m_itemsList;
    foreach (QGraphicsItem * item, selected)
        this->addItem(item);
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

void QGraphicsSelectionItem::addToShape(const QPainterPath & path)
{
    m_shape.translate(pos());
    m_shape = m_shape.united(path);
    setPos(m_shape.boundingRect().topLeft());
    resize(m_shape.boundingRect().size());
    m_shape.translate(-pos());
    QRectF tempRect = m_shape.boundingRect();
    m_rot_widget->setPos(m_shape.boundingRect().center()-QPointF(10,10));
    m_rot_widget->setVisible(tempRect.isValid());
}

void QGraphicsSelectionItem::removeFromShape(const QPainterPath & path)
{
    m_shape.translate(pos());
    m_shape = m_shape.subtracted(path);
    setPos(m_shape.boundingRect().topLeft());
    resize(m_shape.boundingRect().size());
    m_shape.translate(-pos());
    QRectF tempRect = m_shape.boundingRect();
    m_rot_widget->setPos(m_shape.boundingRect().center()-QPointF(10,10));
    m_rot_widget->setVisible(tempRect.isValid());
}

QRectF QGraphicsSelectionItem::boundingRect() const
{
    QPainterPath temp = m_shape;
     if (m_rot_widget->isVisible())
         temp += m_rot_widget->shape().translated(m_rot_widget->pos());
    return temp.boundingRect();
}

bool QGraphicsSelectionItem::contains(const QPointF & point) const
{
    QPainterPath temp = m_shape;
     if (m_rot_widget->isVisible())
         temp += m_rot_widget->opaqueArea().translated(m_rot_widget->pos());
    return temp.contains(point);
}

QPainterPath QGraphicsSelectionItem::opaqueArea() const
{
    QPainterPath temp = m_shape;
    if (m_rot_widget->isVisible())
        temp += m_rot_widget->opaqueArea().translated(m_rot_widget->pos());
    return temp;
}

QPainterPath QGraphicsSelectionItem::shape() const
{
    QPainterPath temp = m_shape;
    if (m_rot_widget->isVisible())
        temp += m_rot_widget->shape().translated(m_rot_widget->pos());
    return temp;
}
