#include "BlurPhotoEffect.h"

#include <QtTreePropertyBrowser>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

BlurPhotoEffect::BlurPhotoEffect(int radius, QObject * parent) :
    AbstractPhotoEffect(i18n("Blur"), parent),
    m_radius(radius)
{
}

QImage BlurPhotoEffect::apply(const QImage & image)
{
    return blurred(image, image.rect(), m_radius);
}

QSharedPointer<QtAbstractPropertyBrowser> BlurPhotoEffect::propertyBrowser() const
{
    QtTreePropertyBrowser * browser = new QtTreePropertyBrowser();

    return QSharedPointer<QtAbstractPropertyBrowser>(browser);
}
