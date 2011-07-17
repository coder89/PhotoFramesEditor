#include "BlurPhotoEffect.h"
#include "BlurPhotoEffect_p.h"

#include <klocalizedstring.h>

BlurPhotoEffectFactory::BlurPhotoEffectFactory(QObject * parent) :
    AbstractPhotoEffectFactory(parent)
{
}

AbstractPhotoEffectInterface * BlurPhotoEffectFactory::getEffectInstance()
{
    return new BlurPhotoEffect(this);
}

QString BlurPhotoEffectFactory::effectName() const
{
    return i18n("Blur effect");
}

Q_EXPORT_PLUGIN2(BlurPhotoEffectFactory, BlurPhotoEffectFactory)
