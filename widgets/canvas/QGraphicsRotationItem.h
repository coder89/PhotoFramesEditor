#ifndef QGRAPHICSSELECTIONITEM_P_H
#define QGRAPHICSSELECTIONITEM_P_H

#include <qmath.h>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

namespace KIPIPhotoFramesEditor
{
    class QGraphicsEditingWidget;

    class RotationHandler : public QGraphicsWidget
    {
            Q_OBJECT

            QPainterPath m_path;
            QPointF m_initial_position;

        public:

            RotationHandler(QGraphicsItem * parent);
            virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
            virtual QPainterPath shape() const;
            virtual QPainterPath opaqueArea() const;
            virtual QRectF boundingRect() const;
            void reset()
            {
                m_rotation_angle = 0;
                m_last_rotation_angle = 0;
            }

        protected:

            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

        Q_SIGNALS:

            void rotationChanged(qreal angle, bool round);

        private:

            qreal m_rotation_angle;
            qreal m_last_rotation_angle;
    };

// ####################################################################################################################

    class QGraphicsRotationItem : public QGraphicsWidget
    {
            Q_OBJECT

            QPointF m_rot_point;

            QPainterPath m_path;
            QPainterPath m_elipse_path;
            QPointF m_initial_position;

            RotationHandler * m_handler;

        public:

            QGraphicsRotationItem(QGraphicsItem * parent = 0);
            virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
            virtual QPainterPath shape() const;
            virtual QPainterPath opaqueArea() const;
            virtual QRectF boundingRect() const;
            virtual bool contains(const QPointF &point) const
            {
                qDebug() << "contains: " << QGraphicsWidget::contains(point);
                return QGraphicsWidget::contains(point);
            }
            QPointF rotationPoint() const
            {
                return m_rot_point+QPoint(10,10);
            }
            void reset()
            {
                m_handler->setPos(100,5);
                m_rot_point = pos();
            }
            void center(const QRectF & bounds)
            {
                this->setPos(bounds.center()-QPoint(10,10));
            }

        protected:

            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

        Q_SIGNALS:

            void rotationChanged(qreal angle, bool round);

        protected Q_SLOTS:

            void emitRotationChanges(qreal deg, bool round);

        friend class QGraphicsEditingWidget;
    };

}

#endif // QGRAPHICSSELECTIONITEM_P_H
