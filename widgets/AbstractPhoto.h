#ifndef ABSTRACT_PHOTO_H
#define ABSTRACT_PHOTO_H

// Qt
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QIcon>

// Local
#include "canvasmouseevent.h"

namespace KIPIPhotoFramesEditor
{
    class Scene;
    class LayersModelItem;
    class PhotoEffectsGroup;

    class AbstractPhoto : public QGraphicsPixmapItem
    {
            QString m_name;

        public:

            void setupWidget(const QPainterPath & path);
            void setupWidget(const QPixmap & photo);

            virtual bool contains(const QPointF & point) const
            {
                return m_complete_path.contains(point);
            }

            virtual QPainterPath shape() const
            {
                return m_complete_path;
            }

            virtual QPainterPath opaqueArea() const
            {
                return m_complete_path;
            }

            virtual QRectF boundingRect() const
            {
                return m_complete_path.boundingRect();
            }

            static const QColor SELECTED_ITEM_COLOR;

            void setGridLines(qreal /*x*/, qreal/* y*/)
            {}

            /// Name of layer [Getter/Setter]
            void setName(const QString & name)
            {
                if (name.isEmpty())
                    return;
                m_name = name;
            }

            QString name() const
            {
                return m_name;
            }

            /// Border width [Getter/Setter]
            void setBorderWidth(qreal width)
            {
                if (width >= 0)
                {
                    m_border_width = width;
                    recalcShape();
                    this->update(this->boundingRect());
                }
            }
            int borderWidth() const
            {
                return m_border_width;
            }

            /// Border color [Getter/Setter]
            void setBorderColor(const QColor & color)
            {
                m_border_color = color;
                this->update(this->boundingRect());
            }
            QColor borderColor() const
            {
                return m_border_color;
            }

            /// Border corners style [Getter/Setter]
            void setBorderCornersStyle(Qt::PenJoinStyle cornersStyle)
            {
                m_border_corner_style = cornersStyle;
                recalcShape();
                this->update(this->boundingRect());
            }
            Qt::PenJoinStyle borderCornersStyle() const
            {
                return m_border_corner_style;
            }

          /**
            * Icon getters
            */
            QIcon & icon()
            {
                return m_icon;
            }
            const QIcon & icon() const
            {
                return m_icon;
            }

            /// Effects group object
            PhotoEffectsGroup * effectsGroup() const
            {
                return m_effects_group;
            }

            void refreshPixmap();

        protected:

            explicit AbstractPhoto(QGraphicsScene * scene = 0);
            virtual ~AbstractPhoto();

            // For widgets drawing
            static AbstractPhoto * getInstance() { return 0; }

            // Recalculate item shape
            void recalcShape();

            // Mouse events
            virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dropEvent(QGraphicsSceneDragDropEvent * event);
            virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
            virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
            virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
            virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

            // Paining method
            virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

            // Pixmap data
            QPixmap m_pixmap_original;
            QPixmap m_pixmap;
            QPixmap & pixmap()
            {
                return m_pixmap;
            }
            const QPixmap & pixmap() const
            {
                return m_pixmap;
            }
            void setPixmap(const QPixmap & pixmap)
            {
                this->setupWidget(pixmap);
            }

            // Icon
            QIcon m_icon;
            void updateIcon();

            // Widget path
            QPainterPath m_image_path;
            QPainterPath m_border_path;
            QPainterPath m_complete_path;

            // Photo resizer class
            class AbstractPhotoResizer;
            friend class AbstractPhotoResizer;

        protected Q_SLOTS:

            // Model item synchronization
            virtual void selectStateChanged(bool state);
            virtual void showStateChanged(bool state);
            virtual void lockStateChanged(bool state);

        private:

            PhotoEffectsGroup * m_effects_group;

            qreal m_border_width;
            Qt::PenJoinStyle m_border_corner_style;
            QColor m_border_color;

            qreal x_grid;
            qreal y_grid;

            friend class Scene;
    };
}

#endif // ABSTRACT_PHOTO_H
