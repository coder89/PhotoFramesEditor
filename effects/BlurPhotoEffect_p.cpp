#include "BlurPhotoEffect_p.h"

#include <klocalizedstring.h>

BlurPhotoEffect::BlurPhotoEffect(QObject * parent) :
    AbstractPhotoEffectInterface(parent)
{
    AbstractPhotoEffectProperty * radius = new AbstractPhotoEffectProperty("Radius");
    radius->value = 10;
    radius->data.insert(AbstractPhotoEffectProperty::Maximum,200);
    radius->data.insert(AbstractPhotoEffectProperty::Minimum,0);
    m_properties.push_back(radius);
}

QImage BlurPhotoEffect::apply(const QImage & image) const
{
    int tempRadius = radius();
    if (!tempRadius)
        return image;
    QImage result = image;
    QPainter p(&result);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawImage(0,0,AbstractPhotoEffectInterface::apply(blurred(image, image.rect(), tempRadius)));
    return result;
}

QString BlurPhotoEffect::effectName() const
{
    return i18n("Blur effect");
}

QString BlurPhotoEffect::toString() const
{
    return effectName();
}

BlurPhotoEffect::operator QString() const
{
    return toString();
}
