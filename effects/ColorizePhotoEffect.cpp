#include "ColorizePhotoEffect.h"
#include "ColorizePhotoEffect_p.h"

#include <klocalizedstring.h>

ColorizePhotoEffectFactory::ColorizePhotoEffectFactory(QObject * parent) :
    AbstractPhotoEffectFactory(parent)
{
}

AbstractPhotoEffectInterface * ColorizePhotoEffectFactory::getEffectInstance()
{
    return new ColorizePhotoEffect(this, this);
}

QString ColorizePhotoEffectFactory::effectName() const
{
    return i18n("Colorize effect");
}

QDomElement ColorizePhotoEffectFactory::toSvg(AbstractPhotoEffectInterface * effect, QDomDocument & document)
{}

AbstractPhotoEffectInterface * ColorizePhotoEffectFactory::fromSvg(QDomElement & element)
{}

Q_EXPORT_PLUGIN2(ColorizePhotoEffectFactory, ColorizePhotoEffectFactory)
