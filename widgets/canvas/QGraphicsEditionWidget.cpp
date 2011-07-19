#include "QGraphicsEditionWidget.h"
#include "QGraphicsRotationItem.h"
#include "QGraphicsSelectionItem.h"
#include "GraphicsEditingWidgetMenu.h"

#include <kmenu.h>
#include <klocalizedstring.h>

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
    connect(m_rot,SIGNAL(rotationAngleChanged(qreal,bool)),this,SLOT(setRotationAngle(qreal,bool)));
    connect(m_rot,SIGNAL(rotationPointChanged(QPointF)),this,SLOT(setRotationPoint(QPointF)));
    setRotationVisible(false);
}

QGraphicsEditionWidget::~QGraphicsEditionWidget()
{
    qDeleteAll(this->childItems());
}

//#####################################################################################################

void QGraphicsEditionWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    GraphicsEditingWidgetMenu menu(this);
    menu.exec(event->screenPos());
}

//#####################################################################################################

void QGraphicsEditionWidget::setSelection(const QList<QGraphicsItem*> & itemList)
{
    qDebug() << "--------------------------------------";
    QSet<QGraphicsItem*> itemSet = itemList.toSet();
    itemSet.remove(m_rot);
    qDebug() << itemSet;
    qDebug() << m_sel->selection();
    if (itemSet != m_sel->selection())
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
    m_rot->reset();
    m_rot->setPos(m_sel->boundingRect().center());
}

void QGraphicsEditionWidget::reset()
{
    setRotationVisible(m_rot_visibility);
}

void QGraphicsEditionWidget::refresh()
{
    qDebug() << m_sel->setSelection(m_sel->selection());
    qDebug() << m_sel->shape();
    setPos(m_sel->setSelection(m_sel->selection()));
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
    {
        result += m_rot->shape().translated(m_rot->pos());
    }
    return result;
}

void QGraphicsEditionWidget::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsWidget::mousePressEvent(event);
    if (event->isAccepted())
    {
        qDebug() << "pressed";
    }
    m_sel->beginMoveUndoCommand();
}

void QGraphicsEditionWidget::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsWidget::mouseMoveEvent(event);
    if (event->buttons() & Qt::LeftButton)
        m_sel->mouseMoveEvent(event);
}

void QGraphicsEditionWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsWidget::mouseReleaseEvent(event);
    if (event->isAccepted())
    {
        qDebug() << "release";
    }
    m_sel->endMoveUndoCommand();
}

void QGraphicsEditionWidget::setRotationAngle(qreal angle, bool round)
{
    m_sel->setRotation(angle, m_rot_point, round);
}

void QGraphicsEditionWidget::setRotationPoint(const QPointF & point)
{
    m_rot_point = point;
}

void QGraphicsEditionWidget::deleteSelected()
{
    if(m_sel->boundingRect().isValid())
    {
        emit deleteSelectedItems();
    }
}

int QGraphicsEditionWidget::maxSceneZValue() const
{
    QList<QGraphicsItem*> items = this->scene()->items();
    items.removeAll(const_cast<QGraphicsEditionWidget*>(this));
    if (items.count())
        items.at(0)->zValue();
    return -1;
}
