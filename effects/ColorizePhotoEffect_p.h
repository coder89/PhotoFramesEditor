#ifndef COLORIZEPHOTOEFFECT_P_H
#define COLORIZEPHOTOEFFECT_P_H

#include "ColorizePhotoEffect_global.h"
#include "AbstractPhotoEffectInterface.h"

using namespace KIPIPhotoFramesEditor;

class ColorizePhotoEffectFactory;

class ColorizePhotoEffect : public AbstractPhotoEffectInterface
{
        Q_INTERFACES(AbstractPhotoEffectInterface)

        static QColor m_last_color;

    public:

        explicit ColorizePhotoEffect(ColorizePhotoEffectFactory * factory, QObject * parent = 0);
        virtual QImage apply(const QImage & image) const;
        virtual QString toString() const;
        virtual operator QString() const;
        QColor color() const
        {
            foreach (AbstractPhotoEffectProperty * property, m_properties)
                if (property->id == "Color")
                    return (m_last_color = property->value.value<QColor>());
            return Qt::transparent;
        }

    private:

        static inline QImage colorized(const QImage & image, const QColor & color)
        {
            QImage result = image;
            unsigned int pixels = result.width() * result.height();
            unsigned int * data = (unsigned int *) result.bits();
            for (unsigned int i = 0; i < pixels; ++i)
            {
                int val = qGray(data[i]);
                data[i] = qRgb(val,val,val);
            }
            QPainter p(&result);
            p.setCompositionMode(QPainter::CompositionMode_Overlay);
            p.fillRect(result.rect(),color);
            p.end();
            return result;
        }
};

#endif // COLORIZEPHOTOEFFECT_P_H
