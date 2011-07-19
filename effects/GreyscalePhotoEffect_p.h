#ifndef GREYSCALEPHOTOEFFECT_P_H
#define GREYSCALEPHOTOEFFECT_P_H

#include "GreyscalePhotoEffect_global.h"
#include "AbstractPhotoEffectInterface.h"

using namespace KIPIPhotoFramesEditor;

class GreyscalePhotoEffect : public AbstractPhotoEffectInterface
{
        Q_INTERFACES(AbstractPhotoEffectInterface)

    public:

        explicit GreyscalePhotoEffect(QObject * parent = 0);
        virtual QImage apply(const QImage & image) const;
        virtual QString effectName() const;
        virtual QString toString() const;
        virtual operator QString() const;

    private:
        static inline QImage greyscaled(const QImage & image)
        {
            QImage result = image;
            unsigned int pixels = result.width() * result.height();
            unsigned int * data = (unsigned int *) result.bits();
            for (unsigned int i = 0; i < pixels; ++i)
            {
                int val = qGray(data[i]);
                data[i] = qRgb(val,val,val);
            }
            return result;
        }
};

#endif // GREYSCALEPHOTOEFFECT_P_H
