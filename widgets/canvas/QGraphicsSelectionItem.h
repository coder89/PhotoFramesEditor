#ifndef QGRAPHICSSELECTIONITEM_H
#define QGRAPHICSSELECTIONITEM_H

#include <QSet>
#include <QGraphicsWidget>
#include <QGraphicsItem>
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

namespace KIPIPhotoFramesEditor
{
    class AbstractPhoto;
    class QGraphicsEditionWidget;

    class QGraphicsSelectionItem : public QGraphicsWidget
    {
            Q_OBJECT

        public:

            QGraphicsSelectionItem(QGraphicsItem * parent = 0);

        public:

            QPointF setSelection(const QSet<QGraphicsItem*> & items);

            virtual QRectF boundingRect() const;
            virtual bool contains(const QPointF & point) const;
            virtual QPainterPath opaqueArea() const;
            virtual QPainterPath shape() const;

            virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
            {
                painter->setPen(Qt::DashLine);
                painter->drawRect(m_shape.boundingRect());
                painter->drawPath(m_shape);
            }

            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

        public Q_SLOTS:

            void setRotation(qreal angle, const QPointF & rotPoint, bool round);

        Q_SIGNALS:

            void selectionChanged();

        private:

            void addToShape(const QPainterPath & path);
            void removeFromShape(const QPainterPath & path);

            QPainterPath calcShape() const;
            QPointF setupWidget();

            QSet<QGraphicsItem*> m_itemsList;
            QPainterPath m_shape;
            int m_flags;

        friend class QGraphicsEditionWidget;
    };
}

#endif // QGRAPHICSSELECTIONITEM_H
