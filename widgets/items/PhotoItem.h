#ifndef PHOTOITEM_H
#define PHOTOITEM_H

#include "AbstractPhoto.h"

namespace KIPIPhotoFramesEditor
{
    class PhotoItemPrivate;
    class PhotoItemPixmapChangeCommand;

    class PhotoItem : public AbstractPhoto
    {
            Q_OBJECT

        public:

            PhotoItem(const QImage & photo = QImage());
            virtual ~PhotoItem();

            /// Convert photo item to SVG format
            virtual QDomElement toSvg(QDomDocument & document) const;

            /// Create Photo item from SVG format code
            static PhotoItem * fromSvg(QDomElement & element);

            /// Pixmap data
            QPixmap & pixmap()
            {
                return m_pixmap;
            }
            const QPixmap & pixmap() const
            {
                return m_pixmap;
            }
            void setPixmap(const QPixmap & pixmap);

            /// Reimplemented from QGraphicsItem
            virtual bool contains(const QPointF & point) const
            {
                return m_complete_path.contains(point);
            }

            /// Reimplemented from AbstractPhoto
            virtual QPainterPath itemShape() const
            {
                if (this->cropShape().isEmpty())
                    return m_image_path;
                else
                    return m_image_path & this->cropShape();
            }

            /// Reimplemented from AbstractPhoto
            virtual QPainterPath itemOpaqueArea() const
            {
                if (this->cropShape().isEmpty())
                    return m_image_path;
                else
                    return m_image_path & this->cropShape();
            }

            /// Reimplemented from AbstractPhoto
            virtual QPainterPath itemDrawArea() const
            {
                return m_image_path;
            }

            /// Returns item's property browser
            virtual QtAbstractPropertyBrowser * propertyBrowser();

        protected:

            /// Converts item data to SVG format
            virtual QDomElement svgVisibleArea(QDomDocument & document) const;

            virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent * event);
            virtual void dropEvent(QGraphicsSceneDragDropEvent * event);

            /// Updates item icon
            virtual void updateIcon();

            /// Scales image to fit scenes rect
            void fitToRect(const QRect & rect);

            /// Reimplemented from AbstractPhoto
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

        private:

            // Refreshes items data
            virtual void refreshItem();

            // Setups items
            void setupItem(const QPixmap & photo);

            // Recalculates item shape
            void recalcShape();

            // Highlight item
            Q_PROPERTY(bool m_highlight READ highlightItem WRITE setHighlightItem)
            bool highlightItem();
            void setHighlightItem(bool isHighlighted);
            bool m_highlight;

            PhotoItemPrivate * d;

            Q_PROPERTY(QPixmap m_pixmap_original READ pixmap WRITE setPixmap)
            QPixmap m_pixmap_original;
            QString m_file_path;

            QPixmap m_pixmap;

            // Widget path
            QPainterPath m_complete_path;
            QPainterPath m_image_path;

        friend class Scene;
        friend class PhotoItemPixmapChangeCommand;
    };
}

#endif // PHOTOITEM_H
