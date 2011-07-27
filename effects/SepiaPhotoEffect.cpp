#include "SepiaPhotoEffect.h"
#include "SepiaPhotoEffect_p.h"

#include <klocalizedstring.h>

SepiaPhotoEffectFactory::SepiaPhotoEffectFactory(QObject * parent) :
    AbstractPhotoEffectFactory(parent)
{
}

AbstractPhotoEffectInterface * SepiaPhotoEffectFactory::getEffectInstance()
{
    return new SepiaPhotoEffect(this, this);
}

QString SepiaPhotoEffectFactory::effectName() const
{
    return i18n("Sepia effect");
}

void SepiaPhotoEffectFactory::writeToSvg(AbstractPhotoEffectInterface * /*effect*/, QDomElement & /*effectElement*/)
{
}

AbstractPhotoEffectInterface * SepiaPhotoEffectFactory::readFromSvg(QDomElement & /*element*/)
{
    SepiaPhotoEffect * effect = (SepiaPhotoEffect*) this->getEffectInstance();
    return effect;
}

Q_EXPORT_PLUGIN2(SepiaPhotoEffectFactory, SepiaPhotoEffectFactory)
