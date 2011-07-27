#include "GreyscalePhotoEffect_p.h"
#include "GreyscalePhotoEffect.h"

#include <klocalizedstring.h>

GreyscalePhotoEffect::GreyscalePhotoEffect(GreyscalePhotoEffectFactory * factory, QObject * parent) :
    AbstractPhotoEffectInterface(factory, parent)
{
}

QImage GreyscalePhotoEffect::apply(const QImage & image) const
{
    if (!strength())
        return image;
    QImage result = image;
    QPainter p(&result);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawImage(0,0,AbstractPhotoEffectInterface::apply(greyscaled(image)));
    return result;
}

QString GreyscalePhotoEffect::effectName() const
{
    return factory()->effectName();
}

QString GreyscalePhotoEffect::toString() const
{
    return effectName();
}

GreyscalePhotoEffect::operator QString() const
{
    return toString();
}
