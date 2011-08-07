#ifndef BLENDBORDERDRAWER_P_H
#define BLENDBORDERDRAWER_P_H

#include "BorderDrawerInterface.h"

#include <QColor>

using namespace KIPIPhotoFramesEditor;

class BlendBorderDrawerFactory;
class BlendBorderDrawer : public BorderDrawerInterface
{
        Q_OBJECT
        Q_INTERFACES(KIPIPhotoFramesEditor::BorderDrawerInterface)

        int m_width;
        Qt::PenJoinStyle m_corners_style;
        QPainterPath m_path;
        QImage m_pixmap;

        static QMap<const char *,QString> m_properties;
        static QMap<Qt::PenJoinStyle, QString> m_corners_style_names;
        static int m_default_width;
        static Qt::PenJoinStyle m_default_corners_style;

    public:

        explicit BlendBorderDrawer(BlendBorderDrawerFactory * factory, QObject * parent = 0);

        virtual QPainterPath path(const QPainterPath & path);

        virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option);

        virtual QString propertyName(const QMetaProperty & property) const;

        virtual QVariant propertyValue(const QString & propertyName) const;

        virtual void setPropertyValue(const QString & propertyName, const QVariant & value);

        virtual QDomElement toSvg(QDomDocument & document) const;

        virtual QString toString() const;

        virtual operator QString() const;

        Q_PROPERTY(int width READ width WRITE setWidth)
        int width() const
        {
            return m_width;
        }
        void setWidth(int width)
        {
            if (width > 0)
                m_width = width;
        }

        Q_PROPERTY(QString corners_style READ cornersStyle WRITE setCornersStyle)
        QString cornersStyle() const
        {
            return m_corners_style_names.value(m_corners_style);
        }
        void setCornersStyle(const QString & cornersStyle)
        {
            m_corners_style = m_corners_style_names.key(cornersStyle);
        }

        virtual QVariant stringNames(const QMetaProperty & property);
        virtual QVariant minimumValue(const QMetaProperty & property);
        virtual QVariant maximumValue(const QMetaProperty & property);
        virtual QVariant stepValue(const QMetaProperty & property);

    private:

    friend class BlendBorderDrawerFactory;
};

#endif // BLENDBORDERDRAWER_P_H
