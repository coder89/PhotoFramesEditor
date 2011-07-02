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
                painter->setPen(QPen(Qt::black, 1, Qt::DotLine));
                painter->drawRect(m_shape.boundingRect());
            }

            virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

        public Q_SLOTS:

            //void setRotationVisible(bool visible = true);

//            void addItem(QGraphicsItem * item)
//            {
//                if (item == this)
//                    return;
//                if (*(m_itemsList.insert(item)) == item)
//                {
//                    QPainterPath temp = item->shape();
//                    temp.translate(item->pos());
//                    //addToShape(temp);
//                }
//            }

//            void removeItem(QGraphicsItem * item)
//            {
//                if (item == this)
//                    return;
//                if (m_itemsList.remove(item))
//                {
//                    QPainterPath temp = item->shape();
//                    temp.translate(item->pos());
//                    //removeFromShape(temp);
//                }
//            }

            QPointF setRotation(qreal angle, const QPointF & rotPoint, bool round);

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
