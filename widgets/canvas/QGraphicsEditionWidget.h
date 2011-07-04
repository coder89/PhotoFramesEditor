#ifndef QGRAPHICSEDITIONWIDGET_H
#define QGRAPHICSEDITIONWIDGET_H

#include <QGraphicsWidget>
#include <QPainter>
namespace KIPIPhotoFramesEditor
{
    class QGraphicsRotationItem;
    class QGraphicsSelectionItem;

    class QGraphicsEditionWidget : public QGraphicsWidget
    {
            Q_OBJECT

            QPointF m_rot_point;

            QGraphicsRotationItem * m_rot;
            QGraphicsSelectionItem * m_sel;

            bool m_rot_visibility;

        public:

            explicit QGraphicsEditionWidget(QGraphicsItem * parent = 0);
            void setSelection(const QList<QGraphicsItem*> & itemList);
            void setRotationVisible(bool visible);
            virtual QRectF boundingRect() const;
            virtual bool contains(const QPointF & point) const;
            virtual QPainterPath opaqueArea() const;
            virtual QPainterPath shape() const;
            void reset();
            virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
            {
                painter->setPen(Qt::red);
                painter->drawPath(shape());
            }

        signals:

        public slots:

            void setRotationAngle(qreal angle, bool round);
            void setRotationPoint(const QPointF & point);

        protected Q_SLOTS:

            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

    };
}

#endif // QGRAPHICSEDITIONWIDGET_H
