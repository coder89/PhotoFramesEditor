#include "ScalingWidgetItem.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::ScalingWidgetItemPrivate
{
    enum
    {
        TopLeft,
        Top,
        TopRight,
        Left,
        Right,
        BottomLeft,
        Bottom,
        BottomRight
    };

    ScalingWidgetItemPrivate() :
        recalculate(true),
        pressedHandler(-1)
    {
        m_handlers[TopLeft] = QRectF(0, 0, 20, 20);
        m_handlers[Top] = QRectF(0, 0, 20, 20);
        m_handlers[TopRight] = QRectF(0, 0, 20, 20);
        m_handlers[Left] = QRectF(0, 0, 20, 20);
        m_handlers[Right] = QRectF(0, 0, 20, 20);
        m_handlers[BottomLeft] = QRectF(0, 0, 20, 20);
        m_handlers[Bottom] = QRectF(0, 0, 20, 20);
        m_handlers[BottomRight] = QRectF(0, 0, 20, 20);
    }

    QTransform currentViewTransform;
    void transformDrawings(const QTransform & viewTransform);
    void calculateDrawings();

    QPainterPath m_scaled_shape;
    QPainterPath m_shape;
    QRectF m_rect;
    QRectF m_handlers[BottomRight+1];
    bool recalculate;
    int pressedHandler;

    friend class ScalingWidgetItem;
};

void ScalingWidgetItemPrivate::transformDrawings(const QTransform & viewTransform)
{
    if (currentViewTransform == viewTransform && !recalculate)
        return;

    m_rect = viewTransform.mapRect( m_scaled_shape.boundingRect() );

    this->calculateDrawings();

    recalculate = false;
    currentViewTransform = viewTransform;
}

void ScalingWidgetItemPrivate::calculateDrawings()
{
    // Scale height of handlers
    qreal h = m_rect.height() - 60;
    h = (h < 0 ? h / 3.0 : 0);
    m_handlers[TopLeft].setHeight(20+h);
    m_handlers[Top].setHeight(20+h);
    m_handlers[TopRight].setHeight(20+h);
    m_handlers[Left].setHeight(20+h);
    m_handlers[Right].setHeight(20+h);
    m_handlers[BottomLeft].setHeight(20+h);
    m_handlers[Bottom].setHeight(20+h);
    m_handlers[BottomRight].setHeight(20+h);

    // Scale width of handlers
    qreal w = m_rect.width() - 60;
    w = (w < 0 ? w / 3.0 : 0);
    m_handlers[TopLeft].setWidth(20+w);
    m_handlers[Top].setWidth(20+w);
    m_handlers[TopRight].setWidth(20+w);
    m_handlers[Left].setWidth(20+w);
    m_handlers[Right].setWidth(20+w);
    m_handlers[BottomLeft].setWidth(20+w);
    m_handlers[Bottom].setWidth(20+w);
    m_handlers[BottomRight].setWidth(20+w);

    m_handlers[TopLeft].moveCenter(m_rect.topLeft());
    m_handlers[Top].moveCenter( QPointF( m_rect.center().x(), m_rect.top() ) );
    m_handlers[TopRight].moveCenter(m_rect.topRight());
    m_handlers[Left].moveCenter( QPointF( m_rect.left(), m_rect.center().y() ) );
    m_handlers[Right].moveCenter( QPointF( m_rect.right(), m_rect.center().y() ) );
    m_handlers[BottomLeft].moveCenter(m_rect.bottomLeft());
    m_handlers[Bottom].moveCenter( QPointF( m_rect.center().x(), m_rect.bottom() ) );
    m_handlers[BottomRight].moveCenter(m_rect.bottomRight());

    m_shape = QPainterPath();
    m_shape.setFillRule(Qt::WindingFill);
    m_shape.addRect(m_rect);
    for (int i = TopLeft; i <= BottomRight; ++i)
        m_shape.addRect(m_handlers[i]);
}

ScalingWidgetItem::ScalingWidgetItem(QGraphicsItem * parent, QGraphicsScene * scene) :
    AbstractItemInterface(parent, scene),
    d(new ScalingWidgetItemPrivate)
{
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, false);
    this->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
}

ScalingWidgetItem::~ScalingWidgetItem()
{
    delete d;
}

QRectF ScalingWidgetItem::boundingRect() const
{
    return d->m_shape.boundingRect();
}

QPainterPath ScalingWidgetItem::opaqueArea() const
{
    return d->m_shape;
}

QPainterPath ScalingWidgetItem::shape() const
{
    return d->m_shape;
}

void ScalingWidgetItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    // Get the view
    QGraphicsView * view = qobject_cast<QGraphicsView*>(widget->parentWidget());
    if (!view)
        return;
    QTransform viewTransform = view->transform();
    d->transformDrawings(viewTransform);

    painter->save();
    painter->setPen(Qt::red);
    painter->setCompositionMode(QPainter::RasterOp_NotSourceAndNotDestination);

    // Draw bounding rect
    painter->drawRect(d->m_rect);

    // Draw handlers
    for (int i = ScalingWidgetItemPrivate::TopLeft; i <= ScalingWidgetItemPrivate::BottomRight; ++i)
        painter->drawRect(d->m_handlers[i]);

    painter->restore();
}

void ScalingWidgetItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    d->pressedHandler = -1;
    if (event->button() == Qt::LeftButton)
    {
        QGraphicsView * view = qobject_cast<QGraphicsView*>(event->widget()->parentWidget());
        QPointF handledPoint = view->transform().map(event->buttonDownPos(Qt::LeftButton));
        for (int i = ScalingWidgetItemPrivate::TopLeft; i <= ScalingWidgetItemPrivate::BottomRight; ++i)
        {
            if (d->m_handlers[i].contains(handledPoint))
            {
                d->pressedHandler = i;
                break;
            }
        }
    }
}

void ScalingWidgetItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsView * view = qobject_cast<QGraphicsView*>(event->widget()->parentWidget());
    QPointF dif = view->transform().map(event->scenePos()) - view->transform().map(event->lastScenePos());
    qreal xFactor = 1;
    qreal yFactor = 1;
    qreal x = 0;
    qreal y = 0;
    switch (d->pressedHandler)
    {
        case ScalingWidgetItemPrivate::TopLeft:
            yFactor = (d->m_rect.height() - dif.y()) / d->m_rect.height();
            xFactor = (d->m_rect.width() - dif.x()) / d->m_rect.width();
            y = dif.y();
            x = dif.x();
            break;
        case ScalingWidgetItemPrivate::Top:
            yFactor = (d->m_rect.height() - dif.y()) / d->m_rect.height();
            y = dif.y();
            break;
        case ScalingWidgetItemPrivate::TopRight:
            yFactor = (d->m_rect.height() - dif.y()) / d->m_rect.height();
            xFactor = (d->m_rect.width() + dif.x()) / d->m_rect.width();
            y = dif.y();
            break;
        case ScalingWidgetItemPrivate::Left:
            xFactor = (d->m_rect.width() - dif.x()) / d->m_rect.width();
            x = dif.x();
            break;
        case ScalingWidgetItemPrivate::Right:
            xFactor = (d->m_rect.width() + dif.x()) / d->m_rect.width();
            break;
        case ScalingWidgetItemPrivate::BottomLeft:
            yFactor = (d->m_rect.height() + dif.y()) / d->m_rect.height();
            xFactor = (d->m_rect.width() - dif.x()) / d->m_rect.width();
            x = dif.x();
            break;
        case ScalingWidgetItemPrivate::Bottom:
            yFactor = (d->m_rect.height() + dif.y()) / d->m_rect.height();
            break;
        case ScalingWidgetItemPrivate::BottomRight:
            yFactor = (d->m_rect.height() + dif.y()) / d->m_rect.height();
            xFactor = (d->m_rect.width() + dif.x()) / d->m_rect.width();
            break;
    }
    QTransform tr(xFactor,                       0,                             0,
                  0,                             yFactor,                       0,
                  x + d->m_rect.x()*(1-xFactor), y + d->m_rect.y()*(1-yFactor), 1);
    d->m_rect = tr.mapRect(d->m_rect);
    d->m_scaled_shape = tr.map(d->m_scaled_shape);
    d->calculateDrawings();
    this->update();
    emit scalingChanged(tr);
}

void ScalingWidgetItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{}

void ScalingWidgetItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{}

void ScalingWidgetItem::setScaleShape(const QPainterPath & shape)
{
    d->m_scaled_shape = shape;
    d->m_rect = d->m_scaled_shape.boundingRect();
    this->setPos( d->m_rect.topLeft() );
    d->m_scaled_shape.translate( -d->m_rect.topLeft() );
    d->m_rect.translate( -d->m_rect.topLeft() );
    d->recalculate = true;
    d->calculateDrawings();
    if (!d->m_rect.isValid())
    {
        this->hide();
        return;
    }
    this->update();
    this->show();
}
