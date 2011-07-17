#include "ColorizePhotoEffect.h"
#include "ColorizePhotoEffect_p.h"

#include <klocalizedstring.h>

ColorizePhotoEffectFactory::ColorizePhotoEffectFactory(QObject * parent) :
    AbstractPhotoEffectFactory(parent)
{
}

AbstractPhotoEffectInterface * ColorizePhotoEffectFactory::getEffectInstance()
{
    return new ColorizePhotoEffect(this);
}

QString ColorizePhotoEffectFactory::effectName() const
{
    return i18n("Colorize effect");
}

Q_EXPORT_PLUGIN2(ColorizePhotoEffectFactory, ColorizePhotoEffectFactory)
