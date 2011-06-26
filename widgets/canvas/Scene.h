#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QPainter>
#include <QColor>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>

// Local
#include "abstract_photo.h"
#include "polygon_widget.h"
#include "canvasmouseevent.h"

namespace KIPIPhotoFramesEditor
{
    class CanvasWidget;
    class Canvas;

    class Scene : protected QGraphicsScene
    {

            Q_OBJECT

            int zIndex;

            enum EditMode
            {
                Disabled,
                LineDrawing,
                WidgetsMoving
            };

        public:

            explicit Scene(const QRectF & dimension, QObject * parent = 0);
            bool removeItems(const QList<QGraphicsItem *> & items);
            void addItem(AbstractPhoto * item);

            void newItem()
            {
                AbstractPhoto * rect;// = new AbstractPhoto(this);
                //rect->moveBy(19,19);
                QPainterPath mypath;
                mypath.closeSubpath();
                mypath.moveTo(0,0);
                mypath.lineTo(0,600);
                mypath.lineTo(160,300);
                mypath.lineTo(160,600);
                mypath.lineTo(100,200);
                mypath.lineTo(10,100);
                mypath.lineTo(160,400);
                rect = new PolygonWidget(mypath, this);
                emit newItemAdded(rect);
                this->children.append(rect);
                rect->setZValue(zIndex++);
                rect->moveBy(190,190);
            }

        Q_SIGNALS:

            void finishEditing(const QPainterPath &);
            void gridChanged(qreal,qreal);
            void newItemAdded(AbstractPhoto*);

        public Q_SLOTS:

            void enableItemsDrawing();
            void disableitemsDrawing();
            void removeSelectedItems();
            void setGrid(qreal x, qreal y);
            void setGridVisible(bool visible);

        protected:

            virtual bool event(QEvent * event);
            virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
            virtual void drawBackground(QPainter * painter, const QRectF & rect);
            virtual void drawForeground(QPainter * painter, const QRectF & rect);
            virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dropEvent(QGraphicsSceneDragDropEvent * event);
            virtual void keyPressEvent(QKeyEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

        private:

            void updateChildernsGrid(qreal x, qreal y);

            EditMode editingMode;
            static const EditMode DEFAULT_EDITING_MODE = WidgetsMoving;

            QGraphicsItem * shadow;

            QGraphicsPathItem * temp_widget;
            QPainterPath temp_path;

            QList<AbstractPhoto*> children;

            // Grid properties
            qreal x_grid;
            qreal y_grid;
            bool  grid_visible;
            QGraphicsItemGroup * grid_item;
            bool grid_changed;

            friend class CanvasWidget;
            friend class Canvas;
    };

}

#endif // CANVAS_H
