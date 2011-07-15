#include "BlurPhotoEffect.h"

#include <QtTreePropertyBrowser>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

BlurPhotoEffect::BlurPhotoEffect(int radius, QObject * parent) :
    AbstractPhotoEffect(i18n("Blur effect"), parent),
    m_radius(radius)
{
}

QImage BlurPhotoEffect::apply(const QImage & image)
{
    return blurred(image, image.rect(), m_radius);
}

QtAbstractPropertyBrowser * BlurPhotoEffect::propertyBrowser() const
{
    QtTreePropertyBrowser * browser = new QtTreePropertyBrowser();

    return browser;
}

QString BlurPhotoEffect::toString() const
{
    return this->name() + " [Radius=" + QString::number(m_radius) + "]" ;
}
