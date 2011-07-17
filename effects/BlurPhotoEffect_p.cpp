#include "BlurPhotoEffect_p.h"

#include <klocalizedstring.h>

BlurPhotoEffect::BlurPhotoEffect(QObject * parent) :
    AbstractPhotoEffectInterface(parent)
{
    Property * radius = new Property("radius");
    radius->name = i18n("Radius");
    radius->value = 10;
    m_properties.push_back(radius);
}

QImage BlurPhotoEffect::apply(const QImage & image) const
{
    QImage result = image;
    QPainter p(&result);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawImage(0,0,AbstractPhotoEffectInterface::apply(blurred(image, image.rect(), radius())));
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
