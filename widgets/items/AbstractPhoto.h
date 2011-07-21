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
#include <QDomNode>

// Local
#include "canvasmouseevent.h"

namespace KIPIPhotoFramesEditor
{
    class Scene;
    class LayersModelItem;
    class PhotoEffectsGroup;
    class AbstractPhotoEffectInterface;

    class AbstractPhoto : public QGraphicsItem
    {
            QString m_name;
            mutable QString m_id;

        public:

            virtual ~AbstractPhoto();

            /** Converts item data into SVG format
              * Each derived class should has its own implementation of this method to save its specific data.
              * \note You should save everything inside your <defs> tag because \class AbstractPhoto's implementation
              * of \fn toSvg() saves presentation data.
              * \note In your implementation you have to call this method to save presentation data in correct format,
              * independendly to your class.
              */
            virtual QDomNode toSvg(QDomDocument & document) const;

            /// Reads item data from SVG structure
            static bool fromSvg(QDomElement & element);

            /// Name of item property
            Q_PROPERTY(QString name READ name WRITE setName)
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
            Q_PROPERTY(qreal m_border_width READ borderWidth WRITE setBorderWidth)
            void setBorderWidth(qreal width)
            {
                if (width >= 0)
                {
                    m_border_width = width;
                    recalcShape();
                    //this->update(this->boundingRect());
                }
            }
            qreal borderWidth() const
            {
                return m_border_width;
            }

            /// Border color [Getter/Setter]
            void setBorderColor(const QColor & color)
            {
                m_border_color = color;
               // this->update(this->boundingRect());
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
               // this->update(this->boundingRect());
            }
            Qt::PenJoinStyle borderCornersStyle() const
            {
                return m_border_corner_style;
            }

            /// Icon of the item [50px x 50px]
            Q_PROPERTY(QIcon m_icon READ icon)
            QIcon & icon()
            {
                return m_icon;
            }
            const QIcon & icon() const
            {
                return m_icon;
            }

            /// Effects group property
            Q_PROPERTY(PhotoEffectsGroup * m_effects_group READ effectsGroup)
            PhotoEffectsGroup * effectsGroup() const
            {
                return m_effects_group;
            }

            Q_PROPERTY(QString m_id READ id WRITE setId)
            QString id() const;

            /// Refreshes item
            virtual void refresh() = 0;

        protected:

            explicit AbstractPhoto(QGraphicsScene * scene = 0);

            // For widgets drawing
            static AbstractPhoto * getInstance() { return 0; }

            /** Returns SVG visible part of data.
              * This is a pure virtual method which should returns QDomElement with part of SVG document
              * with visible data. For example it could be <image> tag if the item describes QGraphicsItem with image
              * or <text> tag if the item is the QGraphicsItem drawing text.
              * This data also should include applied all effects because this data will be directly presented to the user.
              * Data will be also cutted to fit their visual shape.
              */
            virtual QDomElement svgVisibleArea(QDomDocument & document) const = 0;

            static QString pathToString(const QPainterPath & path);

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

            // Sets icon for item
            void setIcon(const QIcon & icon)
            {
                if (icon.isNull())
                    return;
                m_icon = icon;
            }

            // Widget path
            QPainterPath m_border_path;

            // Photo resizer class
            class AbstractPhotoResizer;
            friend class AbstractPhotoResizer;

        private:

            void setupItem();

            PhotoEffectsGroup * m_effects_group;

            qreal m_border_width;
            Qt::PenJoinStyle m_border_corner_style;
            QColor m_border_color;

            qreal x_grid;
            qreal y_grid;

            // Icon object
            QIcon m_icon;

            static const QColor SELECTED_ITEM_COLOR;

            friend class Scene;
            friend class PhotoEffectsGroup;
    };
}

#endif // ABSTRACT_PHOTO_H
