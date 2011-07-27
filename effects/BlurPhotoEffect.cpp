#include "BlurPhotoEffect.h"
#include "BlurPhotoEffect_p.h"

#include <klocalizedstring.h>

BlurPhotoEffectFactory::BlurPhotoEffectFactory(QObject * parent) :
    AbstractPhotoEffectFactory(parent)
{
}

AbstractPhotoEffectInterface * BlurPhotoEffectFactory::getEffectInstance()
{
    return new BlurPhotoEffect(this, this);
}

QString BlurPhotoEffectFactory::effectName() const
{
    return i18n("Blur effect");
}

void BlurPhotoEffectFactory::writeToSvg(AbstractPhotoEffectInterface * effect, QDomElement & effectElement)
{
    BlurPhotoEffect * blurEffect = dynamic_cast<BlurPhotoEffect*>(effect);
    if (blurEffect)
        effectElement.setAttribute(RADIUS_PROPERTY, blurEffect->radius());
}

AbstractPhotoEffectInterface * BlurPhotoEffectFactory::readFromSvg(QDomElement & element)
{
    BlurPhotoEffect * effect = (BlurPhotoEffect*) this->getEffectInstance();
    effect->setRadius( element.attribute(RADIUS_PROPERTY).toInt() );
    return effect;
}

Q_EXPORT_PLUGIN2(BlurPhotoEffectFactory, BlurPhotoEffectFactory)
