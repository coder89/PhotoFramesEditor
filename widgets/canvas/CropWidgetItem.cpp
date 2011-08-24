#include "CropWidgetItem.h"
#include "AbstractPhoto.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

using namespace KIPIPhotoFramesEditor;

class KIPIPhotoFramesEditor::CropWidgetItemPrivate
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

    CropWidgetItemPrivate() :
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

    friend class CropWidgetItem;
};

void CropWidgetItemPrivate::transformDrawings(const QTransform & viewTransform)
{
    if (currentViewTransform == viewTransform && !recalculate)
        return;

    m_rect = currentViewTransform.inverted().mapRect(m_rect);
    m_rect = viewTransform.mapRect(m_rect);

    this->calculateDrawings();

    recalculate = false;
    currentViewTransform = viewTransform;
    qDebug() << "changed" << viewTransform;
}

void CropWidgetItemPrivate::calculateDrawings()
{
    // Scale height of handlers
    qreal h = m_rect.height() - 60;
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
    qreal w = m_rect.width() - 60;
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

    m_handlers[Top][Left].moveCenter(m_rect.topLeft());
    m_handlers[Top][HCenter].moveCenter( QPointF( m_rect.center().x(), m_rect.top() ) );
    m_handlers[Top][Right].moveCenter(m_rect.topRight());
    m_handlers[VCenter][Left].moveCenter( QPointF( m_rect.left(), m_rect.center().y() ) );
    m_handlers[VCenter][Right].moveCenter( QPointF( m_rect.right(), m_rect.center().y() ) );
    m_handlers[Bottom][Left].moveCenter(m_rect.bottomLeft());
    m_handlers[Bottom][HCenter].moveCenter( QPointF( m_rect.center().x(), m_rect.bottom() ) );
    m_handlers[Bottom][Right].moveCenter(m_rect.bottomRight());

    m_elipse = QPainterPath();
    m_elipse.addEllipse(m_rect.center(), 10, 10);

    m_shape = QPainterPath();
    m_shape.setFillRule(Qt::WindingFill);
    m_shape.addRect(m_rect);
    for (int i = Top; i <= Bottom; ++i)
        for (int j = Left; j <= Right; ++j)
            m_shape.addRect(m_handlers[i][j]);
}

CropWidgetItem::CropWidgetItem(QGraphicsItem * parent, QGraphicsScene * scene) :
    AbstractItemInterface(parent, scene),
    d(new CropWidgetItemPrivate)
{
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, false);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    this->setZValue(1.0 / 0.0);
}

CropWidgetItem::~CropWidgetItem()
{
    delete d;
}

QRectF CropWidgetItem::boundingRect() const
{
    return (d->m_crop_shape + d->m_shape).boundingRect();
}

QPainterPath CropWidgetItem::opaqueArea() const
{
    return d->m_crop_shape + d->m_shape;
}

QPainterPath CropWidgetItem::shape() const
{
    return d->m_crop_shape + d->m_shape;
}

void CropWidgetItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    // Get the view
    QGraphicsView * view = qobject_cast<QGraphicsView*>(widget->parentWidget());
    if (!view)
        return;
    QTransform viewTransform = view->transform();
    d->transformDrawings(viewTransform);

    painter->save();

    QPainterPath p;
    p.addPolygon( viewTransform.map( this->mapFromScene(this->scene()->sceneRect()) ) );
    QPainterPath p1;
    p1.addRect(d->m_rect);
    p -= p1;
    painter->fillPath(p, QColor(0, 0, 0, 150));

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

void CropWidgetItem::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Return)
    {
        QPainterPath p;
        p.addRect( d->m_rect );
        qDebug() << "send shape:" << d->currentViewTransform.inverted().map(p).boundingRect();
        emit cropShapeSelected( this->mapToScene( d->currentViewTransform.inverted().map(p) ) );
    }
}

void CropWidgetItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    d->pressedVHandler = -1;
    d->pressedHHandler = -1;
    d->handlerOffset = QPointF(0,0);
    this->setFocus( Qt::MouseFocusReason );
    if (event->button() == Qt::LeftButton)
    {
        QGraphicsView * view = qobject_cast<QGraphicsView*>(event->widget()->parentWidget());
        QPointF handledPoint = view->transform().map(this->mapFromScene(event->buttonDownScenePos(Qt::LeftButton)));
        for (int i = CropWidgetItemPrivate::Top; i <= CropWidgetItemPrivate::Bottom; ++i)
        {
            for (int j = CropWidgetItemPrivate::Left; j <= CropWidgetItemPrivate::Right; ++j)
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
            d->pressedVHandler = CropWidgetItemPrivate::VCenter;
            d->pressedHHandler = CropWidgetItemPrivate::HCenter;
        }
        return;
        found:
            d->handlerOffset = d->m_handlers[d->pressedVHandler][d->pressedHHandler].center() - handledPoint;
    }
}

void CropWidgetItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (d->pressedHHandler == -1 || d->pressedVHandler == -1)
        return;

    QRectF maxRect = d->currentViewTransform.mapRect(d->m_crop_shape.boundingRect());
    QGraphicsView * view = qobject_cast<QGraphicsView*>(event->widget()->parentWidget());
    QPointF point = view->transform().map(event->pos());

    QRectF temp = d->m_rect;

    // Position change
    if (d->pressedVHandler == CropWidgetItemPrivate::VCenter &&
        d->pressedHHandler == CropWidgetItemPrivate::HCenter)
    {
        QPointF dif = view->transform().map(event->scenePos()) - view->transform().map(event->lastScenePos());
        temp.translate(dif);
    }
    // Size change
    else
    {
        // Vertical size change
        if (d->pressedVHandler == CropWidgetItemPrivate::Top)
            temp.setTop( point.y() + d->handlerOffset.y() );
        else if (d->pressedVHandler == CropWidgetItemPrivate::Bottom)
            temp.setBottom( point.y() + d->handlerOffset.y() );

        // Horizontal size change
        if (d->pressedHHandler == CropWidgetItemPrivate::Right)
            temp.setRight( point.x() + d->handlerOffset.x() );
        else if (d->pressedHHandler == CropWidgetItemPrivate::Left)
            temp.setLeft( point.x() + d->handlerOffset.x() );

        // Keeps aspect ratio
        if (event->modifiers() & Qt::ShiftModifier)
        {
            qreal xFactor = temp.width()  / d->m_rect.width();
            qreal yFactor = temp.height() / d->m_rect.height();
            if (xFactor > yFactor || d->pressedHHandler == CropWidgetItemPrivate::HCenter)
            {
                qreal dif = d->m_rect.width() - d->m_rect.width() * yFactor;
                if (d->pressedHHandler == CropWidgetItemPrivate::Right)
                    temp.setRight( d->m_rect.right() - dif );
                else if (d->pressedHHandler == CropWidgetItemPrivate::Left)
                    temp.setLeft( d->m_rect.left() + dif );
                else
                {
                    temp.setRight( d->m_rect.right() - dif / 2.0 );
                    temp.setLeft( d->m_rect.left() + dif / 2.0 );
                }
            }
            else if (xFactor < yFactor || d->pressedVHandler == CropWidgetItemPrivate::VCenter)
            {
                qreal dif = d->m_rect.height() - d->m_rect.height() * xFactor;
                if (d->pressedVHandler == CropWidgetItemPrivate::Top)
                    temp.setTop( d->m_rect.top() + dif );
                else if (d->pressedVHandler == CropWidgetItemPrivate::Bottom)
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
        d->pressedVHandler = (d->pressedVHandler == CropWidgetItemPrivate::Top ? CropWidgetItemPrivate::Bottom :CropWidgetItemPrivate::Top);
    if (temp.width() < 0)
        d->pressedHHandler = (d->pressedHHandler == CropWidgetItemPrivate::Left ? CropWidgetItemPrivate::Right :CropWidgetItemPrivate::Left);

    d->m_rect = temp;

    d->calculateDrawings();
    this->update();
}

void CropWidgetItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{}

void CropWidgetItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{}

void CropWidgetItem::setItems(const QList<AbstractPhoto*> & items)
{
    d->m_items = items;

    d->m_crop_shape = QPainterPath();
    foreach (AbstractPhoto * item, items)
        d->m_crop_shape += item->mapToScene(item->itemDrawArea());

    d->m_rect = QRectF();
    foreach (AbstractPhoto * item, items)
    {
        QRectF t = item->mapRectToScene( item->cropShape().boundingRect() );
        //qDebug() << item->mapRectToScene( item->cropShape().boundingRect() ) << item->cropShape().boundingRect();
        if (!t.isValid())
            t = item->mapToScene(item->itemDrawArea()).boundingRect();
        t.translate( -t.topLeft() );
        d->m_rect = d->m_rect.united( t );
    }

    this->setPos( d->m_crop_shape.boundingRect().topLeft() );
    d->m_crop_shape.translate( -d->m_crop_shape.boundingRect().topLeft() );
    d->recalculate = true;
    d->calculateDrawings();
    if (!d->m_rect.isValid())
    {
        this->hide();
        return;
    }
    this->update();
    this->show();
    qDebug() << this->scenePos() << d->m_rect.topLeft();
}
