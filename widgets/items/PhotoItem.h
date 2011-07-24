#ifndef PHOTOITEM_H
#define PHOTOITEM_H

#include "AbstractPhoto.h"

namespace KIPIPhotoFramesEditor
{
    class PhotoItem : public AbstractPhoto
    {
        public:

            PhotoItem(const QImage & photo = QImage(), QGraphicsScene * scene = 0);

            /// Convert photo item to SVG format
            virtual QDomElement toSvg(QDomDocument & document, bool embedAll) const;

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

            /// Refreshes items data
            virtual void refresh();

            /// Reimplemented from QGraphicsItem
            virtual bool contains(const QPointF & point) const
            {
                return m_complete_path.contains(point);
            }

            /// Reimplemented from QGraphicsItem
            virtual QPainterPath shape() const
            {
                return m_complete_path;
            }

            /// Reimplemented from QGraphicsItem
            virtual QPainterPath opaqueArea() const
            {
                return m_complete_path;
            }

            /// Reimplemented from QGraphicsItem
            virtual QRectF boundingRect() const
            {
                return m_complete_path.boundingRect();
            }

            /// Returns item's property browser
            virtual QtAbstractPropertyBrowser * propertyBrowser();

        protected:

            /// Converts item data to SVG format
            virtual QDomElement svgVisibleArea(QDomDocument & document) const;

            /// Updates item icon
            virtual void updateIcon();

            /// Reimplemented from AbstractPhoto
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

        private:

            // Setups items
            void setupItem(const QPixmap & photo);

            // Recalculates item shape
            void recalcShape();

            Q_PROPERTY(QPixmap m_pixmap_original READ pixmap WRITE setPixmap)
            QPixmap m_pixmap_original;

            QPixmap m_pixmap;

            // Widget path
            QPainterPath m_complete_path;
            QPainterPath m_image_path;
    };
}

#endif // PHOTOITEM_H
