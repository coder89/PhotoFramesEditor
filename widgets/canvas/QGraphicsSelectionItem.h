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
    class QGraphicsSelectionItemPrivate;

    class QGraphicsSelectionItem : public QGraphicsWidget
    {
            Q_OBJECT

            class MoveItemsUndoCommand;

        public:

            QGraphicsSelectionItem(QGraphicsItem * parent = 0);
            ~QGraphicsSelectionItem();

        public:

            QPointF setSelection(const QSet<QGraphicsItem*> & items);
            QSet<QGraphicsItem*> selection() const;

            virtual QRectF boundingRect() const;
            virtual bool contains(const QPointF & point) const;
            virtual QPainterPath opaqueArea() const;
            virtual QPainterPath shape() const;

            virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

        public slots:

            void setRotation(qreal angle, const QPointF & rotPoint, bool round);

        signals:

            void selectionChanged();

        private:

            void addToShape(const QPainterPath & path);
            void removeFromShape(const QPainterPath & path);
            void beginMoveUndoCommand();
            void endMoveUndoCommand();

            QPainterPath calcShape() const;
            QPointF setupWidget();

            QGraphicsSelectionItemPrivate * d;

        friend class QGraphicsEditionWidget;
        friend class QGraphicsSelectionItemPrivate;
    };
}

#endif // QGRAPHICSSELECTIONITEM_H
