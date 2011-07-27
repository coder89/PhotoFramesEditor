#include "GreyscalePhotoEffect.h"
#include "GreyscalePhotoEffect_p.h"

#include <klocalizedstring.h>

GreyscalePhotoEffectFactory::GreyscalePhotoEffectFactory(QObject * parent) :
    AbstractPhotoEffectFactory(parent)
{
}

AbstractPhotoEffectInterface * GreyscalePhotoEffectFactory::getEffectInstance()
{
    return new GreyscalePhotoEffect(this, this);
}

QString GreyscalePhotoEffectFactory::effectName() const
{
    return i18n("Greyscale effect");
}

void GreyscalePhotoEffectFactory::writeToSvg(AbstractPhotoEffectInterface * /*effect*/, QDomElement & /*effectElement*/)
{}

AbstractPhotoEffectInterface * GreyscalePhotoEffectFactory::readFromSvg(QDomElement & /*element*/)
{
    GreyscalePhotoEffect * effect = (GreyscalePhotoEffect*) this->getEffectInstance();
    return effect;
}

Q_EXPORT_PLUGIN2(GreyscalePhotoEffectFactory, GreyscalePhotoEffectFactory)
