#include "ColorizePhotoEffect_p.h"

#include <klocalizedstring.h>
#include <QDebug>
QColor ColorizePhotoEffect::m_last_color = QColor(255,255,255,0);

ColorizePhotoEffect::ColorizePhotoEffect(QObject * parent) :
    AbstractPhotoEffectInterface(parent)
{
    AbstractPhotoEffectProperty  * color = new AbstractPhotoEffectProperty("Color");
    color->value = m_last_color;
    m_properties.push_back(color);
}

QImage ColorizePhotoEffect::apply(const QImage & image) const
{
    QColor tempColor = color();
    if (!strength() || !tempColor.alpha())
        return image;
    QImage result = image;
    QPainter p(&result);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawImage(0,0,AbstractPhotoEffectInterface::apply(colorized(image, tempColor)));
    return result;
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
