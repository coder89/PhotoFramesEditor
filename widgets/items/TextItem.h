#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "AbstractPhoto.h"

namespace KIPIPhotoFramesEditor
{
    class TextItemPrivate;

    class TextItem : public AbstractPhoto
    {
            QColor m_color;
            QFont m_font;

            QPainterPath m_complete_path;
            QPainterPath m_text_path;
            QFontMetrics m_metrics;

            TextItemPrivate * d;

        public:

            TextItem(const QString & text = QString(), QGraphicsScene * scene = 0);
            virtual void focusInEvent(QFocusEvent *event);
            virtual void focusOutEvent(QFocusEvent *event);
            virtual void keyPressEvent(QKeyEvent * event);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

            Q_PROPERTY(QColor m_color READ color WRITE setColor)
            QColor color() const;
            void setColor(const QColor & color);

            Q_PROPERTY(QFont m_font READ font WRITE setFont)
            QFont font() const;
            void setFont(const QFont & font);

            Q_PROPERTY(QString m_text READ text WRITE setText)
            QString text() const;
            void setText(const QString & text);

            virtual QRectF boundingRect() const;
            virtual QPainterPath shape() const;
            virtual QPainterPath opaqueArea() const;
            virtual bool contains(const QPointF & point) const;

            virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
            virtual void refresh();

            virtual QDomElement toSvg(QDomDocument & document, bool embedAll) const;
            virtual QDomElement svgVisibleArea(QDomDocument &document) const;
            static TextItem * fromSvg(QDomElement & element);

            /// Returns item's property browser
            virtual QtAbstractPropertyBrowser * propertyBrowser();

        private:

            QPainterPath getLinePath(const QString & string);
            void setCursorPositionVisible(bool isVisible);
    };
}

#endif // TEXTITEM_H
