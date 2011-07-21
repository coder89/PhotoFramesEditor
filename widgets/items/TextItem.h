#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "AbstractPhoto.h"

namespace KIPIPhotoFramesEditor
{
    class TextItem : public AbstractPhoto
    {
            QColor m_color;
            QFont m_font;
            QString m_text;

            QPainterPath m_complete_path;
            QPainterPath m_text_path;

        public:
            TextItem(const QString & text, QGraphicsScene * scene = 0);

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

            virtual QDomNode toSvg(QDomDocument & document) const;
            virtual QDomElement svgVisibleArea(QDomDocument &document) const;
            static TextItem * fromSvg(QDomElement & element);
    };
}

#endif // TEXTITEM_H
