#include "ColorizePhotoEffect_p.h"

#include <klocalizedstring.h>

ColorizePhotoEffect::ColorizePhotoEffect(QObject * parent) :
    AbstractPhotoEffectInterface(parent)
{
    Property * color = new Property("color");
    color->name = i18n("Color");
    color->value = Qt::white;
    m_properties.push_back(color);
}

QImage ColorizePhotoEffect::apply(const QImage & image) const
{
    return AbstractPhotoEffectInterface::apply(image);
}

QString ColorizePhotoEffect::effectName() const
{
    return i18n("Colorize effect");
}

QString ColorizePhotoEffect::toString() const
{
    return effectName();
}

ColorizePhotoEffect::operator QString() const
{
    return toString();
}
