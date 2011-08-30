#include "ScalingWidgetItem.h"
#include "AbstractPhoto.h"

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
        Top,
        VCenter,
        Bottom
    };

    enum
    {
        Left,
        HCenter,
        Right,
    };

    ScalingWidgetItemPrivate() :
        currentViewTransform(0, 0, 0,    0, 0, 0,   0, 0, 0),
        recalculate(true),
        pressedVHandler(-1),
        pressedHHandler(-1)
    {
        m_handlers[Top][Left] = QRectF(0, 0, 20, 20);
        m_handlers[Top][HCenter] = QRectF(0, 0, 20, 20);
        m_handlers[Top][Right] = QRectF(0, 0, 20, 20);
        m_handlers[VCenter][Left] = QRectF(0, 0, 20, 20);
        m_handlers[VCenter][Right] = QRectF(0, 0, 20, 20);
        m_handlers[Bottom][Left] = QRectF(0, 0, 20, 20);
        m_handlers[Bottom][HCenter] = QRectF(0, 0, 20, 20);
        m_handlers[Bottom][Right] = QRectF(0, 0, 20, 20);
        m_elipse.addEllipse(QPointF(0,0), 10, 10);
    }

    QTransform currentViewTransform;
    void transformDrawings(const QTransform & viewTransform);
    void calculateDrawings();

    QList<AbstractPhoto*> m_items;
    QPainterPath m_crop_shape;
    QPainterPath m_shape;
    QRectF m_rect;
    QRectF m_handlers[Bottom+1][Right+1];
    QPainterPath m_elipse;
    bool recalculate;
    int pressedVHandler;
    int pressedHHandler;
    QPointF handlerOffset;

    friend class ScalingWidgetItem;
};

void ScalingWidgetItemPrivate::transformDrawings(const QTransform & viewTransform)
{
    if (currentViewTransform == viewTransform && !recalculate)
        return;

    m_rect = currentViewTransform.inverted().mapRect(m_rect);
    m_rect = viewTransform.mapRect(m_rect);

    this->calculateDrawings();

    recalculate = false;
    currentViewTransform = viewTransform;
}

void ScalingWidgetItemPrivate::calculateDrawings()
{
    // Scale height of handlers
    qreal h = qAbs(m_rect.height()) - 60;
    h = (h < 0 ? h / 3.0 : 0);
    h = (h < -10 ? -10 : h);
    m_handlers[Top][Left].setHeight(20+h);
    m_handlers[Top][HCenter].setHeight(20+h);
    m_handlers[Top][Right].setHeight(20+h);
    m_handlers[VCenter][Left].setHeight(20+h);
    m_handlers[VCenter][Right].setHeight(20+h);
    m_handlers[Bottom][Left].setHeight(20+h);
    m_handlers[Bottom][HCenter].setHeight(20+h);
    m_handlers[Bottom][Right].setHeight(20+h);

    // Scale width of handlers
    qreal w = qAbs(m_rect.width()) - 60;
    w = (w < 0 ? w / 3.0 : 0);
    w = (w < -10 ? -10 : w);
    m_handlers[Top][Left].setWidth(20+w);
    m_handlers[Top][HCenter].setWidth(20+w);
    m_handlers[Top][Right].setWidth(20+w);
    m_handlers[VCenter][Left].setWidth(20+w);
    m_handlers[VCenter][Right].setWidth(20+w);
    m_handlers[Bottom][Left].setWidth(20+w);
    m_handlers[Bottom][HCenter].setWidth(20+w);
    m_handlers[Bottom][Right].setWidth(20+w);

    m_elipse = QPainterPath();
    m_elipse.addEllipse(m_rect.center(), 18 + w, 18 + h);

    w = qAbs(m_rect.width()) - 35;
    w = (w < 0 ? w / 2.0 : 0);
    h = qAbs(m_rect.height()) - 35;
    h = (h < 0 ? h / 2.0 : 0);
    m_handlers[Top][Left].moveCenter(m_rect.topLeft() + QPointF(w,h));
    m_handlers[Top][HCenter].moveCenter( QPointF( m_rect.center().x(), m_rect.top() + h ) );
    m_handlers[Top][Right].moveCenter(m_rect.topRight() + QPointF(-w,h));
    m_handlers[VCenter][Left].moveCenter( QPointF( m_rect.left() + w, m_rect.center().y() ) );
    m_handlers[VCenter][Right].moveCenter( QPointF( m_rect.right() - w, m_rect.center().y() ) );
    m_handlers[Bottom][Left].moveCenter(m_rect.bottomLeft() + QPointF(w,-h));
    m_handlers[Bottom][HCenter].moveCenter( QPointF( m_rect.center().x(), m_rect.bottom() - h ) );
    m_handlers[Bottom][Right].moveCenter(m_rect.bottomRight() + QPointF(-w,-h));

    m_shape = QPainterPath();
    m_shape.setFillRule(Qt::WindingFill);
    m_shape.addRect(m_rect);
    for (int i = Top; i <= Bottom; ++i)
        for (int j = Left; j <= Right; ++j)
            m_shape.addRect(m_handlers[i][j]);
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

void ScalingWidgetItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * widget)
{
    // Get the view
    QGraphicsView * view = qobject_cast<QGraphicsView*>(widget->parentWidget());
    if (!view)
        return;
    QTransform viewTransform = view->transform();
    d->transformDrawings(viewTransform);

    painter->save();

    // Draw bounding rect
    painter->setCompositionMode(QPainter::RasterOp_NotSourceAndNotDestination);
    painter->setPen(Qt::black);
    painter->setPen(Qt::DashLine);
    painter->drawPath( viewTransform.map(d->m_crop_shape) );
    painter->setPen(Qt::red);
    painter->setPen(Qt::SolidLine);
    painter->drawPath(d->m_shape);
    painter->drawPath(d->m_elipse);

    painter->restore();
}

void ScalingWidgetItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    d->pressedVHandler = -1;
    d->pressedHHandler = -1;
    d->handlerOffset = QPointF(0,0);
    this->setFocus( Qt::MouseFocusReason );
    if (event->button() == Qt::LeftButton)
    {
        QGraphicsView * view = qobject_cast<QGraphicsView*>(event->widget()->parentWidget());
        QPointF handledPoint = view->transform().map(this->mapFromScene(event->buttonDownScenePos(Qt::LeftButton)));
        for (int i = ScalingWidgetItemPrivate::Top; i <= ScalingWidgetItemPrivate::Bottom; ++i)
        {
            for (int j = ScalingWidgetItemPrivate::Left; j <= ScalingWidgetItemPrivate::Right; ++j)
            {
                if (d->m_handlers[i][j].contains(handledPoint))
                {
                    d->pressedVHandler = i;
                    d->pressedHHandler = j;
                    goto found;
                }
            }
        }
        if (d->m_elipse.contains(handledPoint))
        {
            d->pressedVHandler = ScalingWidgetItemPrivate::VCenter;
            d->pressedHHandler = ScalingWidgetItemPrivate::HCenter;
        }
        return;
        found:
            d->handlerOffset = d->m_handlers[d->pressedVHandler][d->pressedHHandler].center() - handledPoint;
    }
}

void ScalingWidgetItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (d->pressedHHandler == -1 || d->pressedVHandler == -1)
        return;

    QRectF maxRect = d->currentViewTransform.mapRect(d->m_crop_shape.boundingRect());
    QGraphicsView * view = qobject_cast<QGraphicsView*>(event->widget()->parentWidget());

    // New handler position calc
    QPointF point = view->transform().map(event->pos()) + d->handlerOffset;
    if (point.rx() < maxRect.left())
        point.setX( maxRect.left() );
    else if (point.rx() > maxRect.right())
        point.setX( maxRect.right() );
    if (point.ry() < maxRect.top())
        point.setY( maxRect.top() );
    else if (point.ry() > maxRect.bottom())
        point.setY( maxRect.bottom() );

    QRectF temp = d->m_rect;

    // Position change
    if (d->pressedVHandler == ScalingWidgetItemPrivate::VCenter &&
        d->pressedHHandler == ScalingWidgetItemPrivate::HCenter)
    {
        QPointF dif = view->transform().map(event->scenePos()) - view->transform().map(event->lastScenePos());
        temp.translate(dif);
    }
    // Size change
    else
    {
        // Vertical size change
        if (d->pressedVHandler == ScalingWidgetItemPrivate::Top)
            temp.setTop( point.y() );
        else if (d->pressedVHandler == ScalingWidgetItemPrivate::Bottom)
            temp.setBottom( point.y() );

        // Horizontal size change
        if (d->pressedHHandler == ScalingWidgetItemPrivate::Right)
            temp.setRight( point.x() );
        else if (d->pressedHHandler == ScalingWidgetItemPrivate::Left)
            temp.setLeft( point.x() );

        // Keeps aspect ratio
        if (event->modifiers() & Qt::ShiftModifier)
        {
            qreal xFactor = temp.width()  / d->m_rect.width();
            qreal yFactor = temp.height() / d->m_rect.height();
            if (xFactor > yFactor || d->pressedHHandler == ScalingWidgetItemPrivate::HCenter)
            {
                qreal dif = d->m_rect.width() - d->m_rect.width() * yFactor;
                if (d->pressedHHandler == ScalingWidgetItemPrivate::Right)
                    temp.setRight( d->m_rect.right() - dif );
                else if (d->pressedHHandler == ScalingWidgetItemPrivate::Left)
                    temp.setLeft( d->m_rect.left() + dif );
                else
                {
                    temp.setRight( d->m_rect.right() - dif / 2.0 );
                    temp.setLeft( d->m_rect.left() + dif / 2.0 );
                }
            }
            else if (xFactor < yFactor || d->pressedVHandler == ScalingWidgetItemPrivate::VCenter)
            {
                qreal dif = d->m_rect.height() - d->m_rect.height() * xFactor;
                if (d->pressedVHandler == ScalingWidgetItemPrivate::Top)
                    temp.setTop( d->m_rect.top() + dif );
                else if (d->pressedVHandler == ScalingWidgetItemPrivate::Bottom)
                    temp.setBottom( d->m_rect.bottom() - dif );
                else
                {
                    temp.setTop( d->m_rect.top() + dif / 2.0 );
                    temp.setBottom( d->m_rect.bottom() - dif / 2.0 );
                }
            }
        }
    }

    temp.setBottom( qMin(temp.bottom() , maxRect.bottom()) );
    temp.setTop( qMax(temp.top() , maxRect.top()) );
    temp.setLeft( qMax(temp.left() , maxRect.left()) );
    temp.setRight( qMin(temp.right() , maxRect.right()) );

    // Rect inverse
    if (temp.height() < 0)
    {
        qreal t = temp.bottom();
        temp.setBottom(temp.top());
        temp.setTop(t);
        d->pressedVHandler = (d->pressedVHandler == ScalingWidgetItemPrivate::Top ? ScalingWidgetItemPrivate::Bottom : ScalingWidgetItemPrivate::Top);
    }
    if (temp.width() < 0)
    {
        qreal t = temp.right();
        temp.setRight(temp.left());
        temp.setLeft(t);
        d->pressedHHandler = (d->pressedHHandler == ScalingWidgetItemPrivate::Left ? ScalingWidgetItemPrivate::Right : ScalingWidgetItemPrivate::Left);
    }

    d->m_rect = temp;

    d->calculateDrawings();
    this->update();

//    QTransform sc;
//    //sc.translate(p.x(), p.y());
//    sc.scale(xFactor, yFactor);
//    //sc.translate(-p.x(), -p.y());
//    qDebug() << this->scenePos();
//    emit scalingChanged(sc);
//    qDebug() << "--------------";
}

void ScalingWidgetItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * /*event*/)
{}

void ScalingWidgetItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * /*event*/)
{}

void ScalingWidgetItem::setScaleShape(const QList<AbstractPhoto*> & items)
{
    d->m_items = items;

    d->m_crop_shape = QPainterPath();
    foreach (AbstractPhoto * item, items)
        d->m_crop_shape += item->mapToScene(item->itemDrawArea());

    this->setPos( d->m_crop_shape.boundingRect().topLeft() );
    d->m_crop_shape.translate( -d->m_crop_shape.boundingRect().topLeft() );
    d->m_rect = d->m_crop_shape.boundingRect();
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
