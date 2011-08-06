#ifndef QGRAPHICSSELECTIONITEM_P_H
#define QGRAPHICSSELECTIONITEM_P_H

#include <qmath.h>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "AbstractItemInterface.h"

namespace KIPIPhotoFramesEditor
{
    class QGraphicsRotationItemPrivate;
    class QGraphicsRotationItem : public AbstractItemInterface
    {
            Q_OBJECT

            QGraphicsRotationItemPrivate * d;

        public:

            QGraphicsRotationItem(QGraphicsItem * parent = 0);
            virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
            virtual QPainterPath shape() const;
            virtual QPainterPath opaqueArea() const;
            virtual QRectF boundingRect() const;
            void setRotatedShape(const QPainterPath & path);
            void reset();
            qreal angle() const;
            QPointF rotationPoint() const;
            bool isRotated() const;

        protected:

            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

        Q_SIGNALS:

            void rotationChanged(const QPointF & point, qreal angle);
            void rotationFinished(const QPointF & point, qreal angle);

        friend class QGraphicsEditingWidget;
    };

}

#endif // QGRAPHICSSELECTIONITEM_P_H
