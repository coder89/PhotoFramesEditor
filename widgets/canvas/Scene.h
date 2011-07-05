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
    class ScenePrivate;

    class Scene : protected QGraphicsScene
    {

            Q_OBJECT

            ScenePrivate * d;

            enum EditMode
            {
                Disabled,
                WidgetsMoving,
                Rotating,
                LineDrawing,
            };

        public:

            explicit Scene(const QRectF & dimension, QObject * parent = 0);
            ~Scene();

            void removeItem(AbstractPhoto * item);
            void removeItems(const QList<AbstractPhoto*> & items);
            void addItem(AbstractPhoto * item);
            QList<AbstractPhoto*> selectedItems() const;
            void setMode(EditMode mode);
            const QGraphicsScene * toGraphicsScene() const
            {
                return this;
            }
            QGraphicsScene * toGraphicsScene()
            {
                return this;
            }


        Q_SIGNALS:

            void finishEditing(const QPainterPath & path);
            void gridChanged(qreal x, qreal y);
            void newItemAdded(AbstractPhoto * item);
            void itemAboutToBeRemoved(AbstractPhoto * item);
            void itemsAboutToBeRemoved(const QList<AbstractPhoto*> & items);

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
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

        private Q_SLOTS:

            void calcSelectionBoundingRect();

        private:

            void updateChildernsGrid(qreal x, qreal y);
            void setRotationWidgetVisible(bool visible);

            EditMode editingMode;
            static const EditMode DEFAULT_EDITING_MODE = WidgetsMoving;

            QGraphicsRectItem * shadow;

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
            friend class ScenePrivate;
    };

}

#endif // CANVAS_H
