#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QPainter>
#include <QColor>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsItem>

// Local
#include "PhotoItem.h"
#include "TextItem.h"
#include "polygon_widget.h"
#include "canvasmouseevent.h"

namespace KIPIPhotoFramesEditor
{
    class Canvas;
    class CanvasWidget;
    class ScenePrivate;
    class MoveItemsUndoCommand;

    class Scene : protected QGraphicsScene
    {

            Q_OBJECT

            ScenePrivate * d;

            enum EditMode
            {
                View = 0,
                Selecting = 1,
                Moving = 2,
                MouseTracking = 4,
                OneclickFocusItems = 8,
                Rotating = 16,
            };

            enum SelectionMode
            {
                NoSelection = 0,
                SingleSelection = 1,
                MultiSelection = 2,
            };

        public:

            explicit Scene(const QRectF & dimension, QObject * parent = 0);
            ~Scene();

            void removeItem(AbstractPhoto * item);
            void removeItems(const QList<AbstractPhoto*> & items);
            void addItem(AbstractPhoto * item);
            QList<AbstractPhoto*> selectedItems() const;
            void setInteractionMode(int mode);
            void setSelectionMode(SelectionMode selectionMode);
            bool isSelectionVisible();
            QDomNode toSvg(QDomDocument & document);
            static Scene * fromSvg(QDomElement & svgImage);

        Q_SIGNALS:

            void finishEditing(const QPainterPath & path);
            void gridChanged(qreal x, qreal y);
            void newItemAdded(AbstractPhoto * item);
            void itemAboutToBeRemoved(AbstractPhoto * item);
            void itemsAboutToBeRemoved(const QList<AbstractPhoto*> & items);
            void mousePressedPoint(const QPointF & point);

        public Q_SLOTS:

            void enableItemsDrawing();
            void disableitemsDrawing();
            void removeSelectedItems();
            void setGrid(qreal x, qreal y);
            void setGridVisible(bool visible);
            bool isGridVisible();
            void updateSelection();
            void setSelectionVisible(bool isVisible);

        protected:

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
            const QGraphicsScene * toGraphicsScene() const
            {
                return this;
            }
            QGraphicsScene * toGraphicsScene()
            {
                return this;
            }

        private Q_SLOTS:

            void calcSelectionBoundingRect();

        private:

            void setRotationWidgetVisible(bool visible);

            int m_interaction_mode;
            static const int DEFAULT_EDITING_MODE = Moving & Selecting;

            SelectionMode selectionMode;
            static const SelectionMode DEFAULT_SELECTING_MODE = MultiSelection;

            QGraphicsPathItem * temp_widget;
            QPainterPath temp_path;

            QList<AbstractPhoto*> children;

            // Grid properties
            qreal x_grid;
            qreal y_grid;
            bool  grid_visible;
            QGraphicsItemGroup * grid_item;
            bool grid_changed;

            static QColor OUTSIDE_SCENE_COLOR;

            friend class Canvas;
            friend class CanvasWidget;
            friend class ScenePrivate;
            friend class AbstractPhoto;
            friend class MoveItemsUndoCommand;
    };

}

#endif // CANVAS_H
