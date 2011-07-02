#ifndef QGRAPHICSEDITIONWIDGET_H
#define QGRAPHICSEDITIONWIDGET_H

#include <QGraphicsWidget>

namespace KIPIPhotoFramesEditor
{
    class QGraphicsRotationItem;
    class QGraphicsSelectionItem;

    class QGraphicsEditionWidget : public QGraphicsWidget
    {
            Q_OBJECT

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

        signals:

        public slots:

            void setRotation(qreal angle, bool round);

        protected Q_SLOTS:

            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

    };
}

#endif // QGRAPHICSEDITIONWIDGET_H
