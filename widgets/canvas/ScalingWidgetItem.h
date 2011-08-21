#ifndef SCALINGWIDGETITEM_H
#define SCALINGWIDGETITEM_H

#include "AbstractItemInterface.h"

namespace KIPIPhotoFramesEditor
{
    class ScalingWidgetItemPrivate;

    class ScalingWidgetItem : public AbstractItemInterface
    {
            Q_OBJECT

            ScalingWidgetItemPrivate * d;

        public:

            ScalingWidgetItem(QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
            virtual ~ScalingWidgetItem();

            virtual QRectF boundingRect() const;
            virtual QPainterPath opaqueArea() const;
            virtual QPainterPath shape() const;
            virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);

            void setScaleShape(const QPainterPath & shape);

        signals:

            void scalingChanged(qreal xFactor, qreal yFactor);
            void scalingChanged(const QTransform & scale);
            void scalingFinished(qreal xFactor, qreal yFactor);

        friend class ScalingWidgetItemPrivate;
    };
}

#endif // SCALINGWIDGETITEM_H
