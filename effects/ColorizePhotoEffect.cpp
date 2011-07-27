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


void ColorizePhotoEffectFactory::writeToSvg(AbstractPhotoEffectInterface * effect, QDomElement & effectElement)
{
    ColorizePhotoEffect * colorizeEffect = dynamic_cast<ColorizePhotoEffect*>(effect);
    if (colorizeEffect)
        effectElement.setAttribute(COLOR_PROPERTY, colorizeEffect->color().name());
}

AbstractPhotoEffectInterface * ColorizePhotoEffectFactory::readFromSvg(QDomElement & element)
{
    ColorizePhotoEffect * effect = (ColorizePhotoEffect*) this->getEffectInstance();
    effect->setColor( QColor(element.attribute(COLOR_PROPERTY)) );
    return effect;
}

Q_EXPORT_PLUGIN2(ColorizePhotoEffectFactory, ColorizePhotoEffectFactory)
