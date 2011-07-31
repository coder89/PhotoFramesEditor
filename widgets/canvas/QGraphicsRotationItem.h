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
    class QGraphicsEditingWidget;

    class RotationHandler : public AbstractItemInterface
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

            void rotationAngleChanged(qreal angle, bool round);

        private:

            qreal m_rotation_angle;
            qreal m_last_rotation_angle;

        friend class QGraphicsRotationItem;
    };

// ####################################################################################################################

    class QGraphicsRotationItem : public AbstractItemInterface
    {
            Q_OBJECT

            QPointF m_rot_point;

            QPainterPath m_path;
            QPainterPath m_elipse_path;
            QPainterPath m_handler_path;
            QPointF m_initial_position;
            QPointF m_handler_pos;

            RotationHandler * m_handler;

            bool m_elipse_pressed;

        public:

            QGraphicsRotationItem(QGraphicsItem * parent = 0);
            virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
            virtual QPainterPath shape() const;
            virtual QPainterPath opaqueArea() const;
            virtual QRectF boundingRect() const;
            void center(const QRectF & boundingRect);
            void reset()
            {
                m_handler_pos = QPointF(100,5);
                m_rot_point = QPointF();
                emit rotationPointChanged(m_rot_point);
            }

        protected:

            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

        Q_SIGNALS:

            void rotationAngleChanged(qreal angle, bool round);
            void rotationPointChanged(const QPointF & point);

        protected Q_SLOTS:

            void emitRotationChanges(qreal deg, bool round);

        friend class QGraphicsEditingWidget;
    };

}

#endif // QGRAPHICSSELECTIONITEM_P_H
