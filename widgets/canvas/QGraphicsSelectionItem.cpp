#include "QGraphicsSelectionItem.h"
#include "AbstractPhoto.h"
#include "global.h"

#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include <QSet>
#include <qmath.h>
#include <QUndoCommand>

#include <kapplication.h>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::QGraphicsSelectionItemPrivate
{
    QGraphicsSelectionItemPrivate() :
        m_shape(),
        m_itemsList(),
        m_flags(0),
        m_move_command(0)
    {}

    QPainterPath m_shape;
    QSet<QGraphicsItem*> m_itemsList;
    int m_flags;

    QGraphicsSelectionItem::MoveItemsUndoCommand * m_move_command;

    friend class QGraphicsSelectionItem;
};

class KIPIPhotoFramesEditor::QGraphicsSelectionItem::MoveItemsUndoCommand : public QUndoCommand
{
        QMap<QGraphicsItem*,QPointF> m_itemsList;
        bool done;
    public:
        MoveItemsUndoCommand(QSet<QGraphicsItem*> selectedItems, QUndoCommand * parent = 0) :
            QUndoCommand(parent),
            done(true)
        {
            foreach (QGraphicsItem * item, selectedItems)
                m_itemsList.insert(item,item->scenePos());
        }
        bool isPositionChanged();
        virtual void redo();
        virtual void undo();
};

bool QGraphicsSelectionItem::MoveItemsUndoCommand::isPositionChanged()
{
    for (QMap<QGraphicsItem*,QPointF>::iterator it = m_itemsList.begin(); it != m_itemsList.end(); ++it)
    {
        if (it.key()->scenePos() != it.value())
            return true;
    }
    return false;
}

void QGraphicsSelectionItem::MoveItemsUndoCommand::redo()
{
    if (!done)
    {
        for (QMap<QGraphicsItem*,QPointF>::iterator it = m_itemsList.begin(); it != m_itemsList.end(); ++it)
        {
            QGraphicsItem * item = it.key();
            QPointF currentPos = item->scenePos();
            item->setPos(item->mapToParent(item->mapFromScene(it.value())));
            it.value() = currentPos;
        }
        done = !done;
    }
}

void QGraphicsSelectionItem::MoveItemsUndoCommand::undo()
{
    if (done)
    {
        for (QMap<QGraphicsItem*,QPointF>::iterator it = m_itemsList.begin(); it != m_itemsList.end(); ++it)
        {
            QGraphicsItem * item = it.key();
            QPointF currentPos = item->scenePos();
            item->setPos(item->mapToParent(item->mapFromScene(it.value())));
            it.value() = currentPos;
        }
        done = !done;
    }
}

QGraphicsSelectionItem::QGraphicsSelectionItem(QGraphicsItem * parent) :
    QGraphicsWidget(parent),
    d(new QGraphicsSelectionItemPrivate)
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
    d->m_itemsList = items;
    return setupWidget();
}

QSet<QGraphicsItem*> QGraphicsSelectionItem::selection() const
{
    return d->m_itemsList;
}

void QGraphicsSelectionItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    painter->setPen(Qt::DashLine);
    painter->drawRect(d->m_shape.boundingRect());
    painter->drawPath(d->m_shape);
}

void QGraphicsSelectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QPointF p = event->pos()-event->lastPos();
    foreach (QGraphicsItem * item, d->m_itemsList)
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
    qDebug() << "mouseReleaseEvent: " << event->isAccepted();
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
    QPainterPath result = d->m_shape;
    return result;
}

QPainterPath QGraphicsSelectionItem::calcShape() const
{
    QPainterPath result;
    foreach (QGraphicsItem * item, d->m_itemsList)
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
    d->m_shape = calcShape();
    QPointF offset = d->m_shape.boundingRect().topLeft();
    d->m_shape.translate(-offset);
    resize(d->m_shape.boundingRect().size());
    return offset;
}

void QGraphicsSelectionItem::setRotation(qreal angle, const QPointF & rotPoint, bool /*round*/)
{
    QPointF point = (rotPoint);
    foreach (QGraphicsItem * item, d->m_itemsList)
    {
        QPointF temp = point;
        qreal x = temp.rx();
        qreal y = temp.ry();
        item->setTransform(item->transform()*(QTransform().translate(x,y).rotate(angle).translate(-x,-y)));
    }
    d->m_shape = this->calcShape();
    d->m_shape.translate(-this->scenePos());
}

void QGraphicsSelectionItem::beginMoveUndoCommand()
{
    if (d->m_move_command)
        delete d->m_move_command;
    d->m_move_command = new MoveItemsUndoCommand(d->m_itemsList);
}

void QGraphicsSelectionItem::endMoveUndoCommand()
{
    if (d->m_move_command)
    {
        if (d->m_move_command->isPositionChanged())
            PFE_PostUndoCommand(d->m_move_command);
        else
            delete d->m_move_command;
        d->m_move_command = 0;
    }
}
