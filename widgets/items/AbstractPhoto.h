#ifndef ABSTRACT_PHOTO_H
#define ABSTRACT_PHOTO_H

// Qt
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QIcon>
#include <QDomNode>
#include <QtAbstractPropertyBrowser>

// Local
#include "AbstractItemInterface.h"
#include "BordersGroup.h"

namespace KIPIPhotoFramesEditor
{
    class Scene;
    class LayersModelItem;
    class PhotoEffectsGroup;
    class AbstractPhotoEffectInterface;

    class AbstractPhoto : public AbstractItemInterface
    {
            Q_OBJECT

            QString m_name;
            mutable QString m_id;

        public:

            virtual ~AbstractPhoto();

            /** Returns item's bounding rectangle.
              * \note This methods shouldn't be reimplemented because it's taking into account borders shape.
              * Reimplement \fn itemShape() and \fn itemOpaqueArea() methods instead.
              */
            virtual QRectF boundingRect() const
            {
                return shape().boundingRect();
            }
            /** Returns item's shape.
              * \note This methods shouldn't be reimplemented because it's taking into account borders shape.
              * Reimplement \fn itemShape() and \fn itemOpaqueArea() methods instead.
              */
            virtual QPainterPath shape() const
            {
                QPainterPath result = this->itemShape();
                return result.united(bordersGroup()->shape());
            }
            /** Returns item's opaque area.
              * \note This methods shouldn't be reimplemented because it's taking into account borders shape.
              * Reimplement \fn itemShape() and \fn itemOpaqueArea() methods instead.
              */
            virtual QPainterPath opaqueArea() const
            {
                QPainterPath result = this->itemOpaqueArea();
                return result.united(bordersGroup()->shape());
            }

            /** Returns item shape
              * Implement this method to return shape of the item.
              * You should take into account only your item's implementation shape, not whith borders or effects shapes.
              * This is done automaticaly by AbstractPhoto class.
              */
            virtual QPainterPath itemShape() const = 0;
            /** Returns item opaque area
              * Implement this method to return opaque area of the item.
              * You should take into account only your item's implementation opaque area, not whith borders or effects opaque areas.
              * This is done automaticaly by AbstractPhoto class.
              */
            virtual QPainterPath itemOpaqueArea() const = 0;
            /** Returns item's draw area.
              * \note This area is uncropped using cropShape()
              */
            virtual QPainterPath itemDrawArea() const = 0;

            /** Converts item data into SVG format
              * Each derived class should has its own implementation of this method to save its specific data.
              * \note You should save everything inside your <defs> tag because \class AbstractPhoto's implementation
              * of \fn toSvg() saves presentation data.
              * \note In your implementation you have to call this method to save presentation data in correct format,
              * independendly to your class.
              */
            virtual QDomElement toSvg(QDomDocument & document) const;

            /// Reads item data from SVG structure
            bool fromSvg(QDomElement & element);

            /// Name of item property
            Q_PROPERTY(QString name READ name WRITE setName)
            void setName(const QString & name)
            {
                if (name.isEmpty())
                    return;
                m_name = name.simplified();
                if (m_name.length() > 20)
                {
                    m_name = m_name.left(20);
                    m_name.append("...");
                }
            }
            QString name() const
            {
                return m_name;
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

            /// Borders group property
            Q_PROPERTY(BordersGroup * m_borders_group READ bordersGroup)
            BordersGroup * bordersGroup() const
            {
                return m_borders_group;
            }

            Q_PROPERTY(QString m_id READ id)
            QString id() const;

            /// Refreshes item
            void refresh()
            {
                this->refreshItem();
                emit changed();
            }

            /// Crops item to shape passed in method's argument
            Q_PROPERTY(QPainterPath m_crop_shape READ cropShape WRITE setCropShape)
            void setCropShape(const QPainterPath & cropShape);
            QPainterPath cropShape() const;

            /// Returns item's property browser
            virtual QtAbstractPropertyBrowser * propertyBrowser() = 0;

        signals:

          /** This signal is emited when item was changed and has been updated.
            * It is used by listeners to update their views and be up to date.
            */
            void changed();

        protected:

            explicit AbstractPhoto();

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

            // Draws abstract item presentation
            virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

            // Mouse events
            virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dropEvent(QGraphicsSceneDragDropEvent * event);
            virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
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

            // Refreshes item's view and internal data
            virtual void refreshItem() = 0;

            void setupItem();

            PhotoEffectsGroup * m_effects_group;
            BordersGroup * m_borders_group;

            qreal m_border_width;
            Qt::PenJoinStyle m_border_corner_style;
            QColor m_border_color;

            qreal x_grid;
            qreal y_grid;

            // Icon object
            QIcon m_icon;

            // Item's crop shape
            QPainterPath m_crop_shape;

            static const QColor SELECTED_ITEM_COLOR;

            friend class Scene;
            friend class PhotoEffectsGroup;
    };
}

#endif // ABSTRACT_PHOTO_H
