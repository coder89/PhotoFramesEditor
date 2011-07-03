#include "QGraphicsEditionWidget.h"
#include "QGraphicsRotationItem.h"
#include "QGraphicsSelectionItem.h"

using namespace KIPIPhotoFramesEditor;

QGraphicsEditionWidget::QGraphicsEditionWidget(QGraphicsItem * parent) :
    QGraphicsWidget(parent),
    m_rot(new QGraphicsRotationItem(this)),
    m_sel(new QGraphicsSelectionItem(this))
{
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable, false);
    this->setHandlesChildEvents(false);
    m_sel->setParent(this);
    m_sel->setZValue(1);
    m_rot->setParent(this);
    m_rot->setZValue(2);
    connect(m_rot,SIGNAL(rotationChanged(qreal,bool)),this,SLOT(setRotation(qreal,bool)));
    setRotationVisible(false);
}

void QGraphicsEditionWidget::setSelection(const QList<QGraphicsItem*> & itemList)
{
    qDebug() << "--------------------------------------";
    QSet<QGraphicsItem*> itemSet = itemList.toSet();
    itemSet.remove(m_rot);
    qDebug() << itemSet;
    qDebug() << m_sel->m_itemsList;
    if (itemSet != m_sel->m_itemsList)
    {
        qDebug() << "Selection changed";
        setPos(m_sel->setSelection(itemSet));
        reset();
    }
    qDebug() << "--------------------------------------";
}

void QGraphicsEditionWidget::setRotationVisible(bool visible)
{
    m_rot_visibility = visible;
    m_rot->setVisible(m_rot_visibility && m_sel->boundingRect().isValid());
    m_rot->center(m_sel->shape().boundingRect());
    m_rot->reset();
}

void QGraphicsEditionWidget::reset()
{
    setRotationVisible(m_rot_visibility);
}

QRectF QGraphicsEditionWidget::boundingRect() const
{
    return shape().boundingRect();
}

bool QGraphicsEditionWidget::contains(const QPointF & point) const
{
    return shape().contains(point);
}

QPainterPath QGraphicsEditionWidget::opaqueArea() const
{
    return shape();
}

QPainterPath QGraphicsEditionWidget::shape() const
{
    QPainterPath result = m_sel->shape();
    if (m_rot->isVisible())
        result = result.united(m_rot->shape().translated(m_rot->pos()));
    return result;
}

void QGraphicsEditionWidget::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (m_sel->contains(event->pos()))
    {
        m_sel->mouseMoveEvent(event);
        QGraphicsWidget::mouseMoveEvent(event);
    }
    else if (m_rot->contains(event->pos()-m_rot->pos()))
    {
        QGraphicsWidget::mouseMoveEvent(event);
    }
    else
        event->setAccepted(false);
}

void QGraphicsEditionWidget::setRotation(qreal angle, bool round)
{
    QPointF p = m_sel->setRotation(angle, m_rot->rotationPoint(), round);
    qDebug() << m_rot->rotationPoint();
    QPointF lp = pos();
    //setPos(p);
    qDebug() << p;
    //m_rot->setPos(m_rot->pos()+lp-p);
}
