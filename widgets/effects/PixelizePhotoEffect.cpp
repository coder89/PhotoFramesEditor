#include "PixelizePhotoEffect.h"

#include <QImage>
#include <QtTreePropertyBrowser>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

PixelizePhotoEffect::PixelizePhotoEffect(int pixelSize, QObject * parent) :
    AbstractPhotoEffect(i18n("Pixelize"), parent),
    m_pixelSize(pixelSize)
{
}

QImage PixelizePhotoEffect::apply(const QImage & image)
{
    return pixelize(image, m_pixelSize);
}

QtAbstractPropertyBrowser * PixelizePhotoEffect::propertyBrowser() const
{
    QtTreePropertyBrowser * browser = new QtTreePropertyBrowser();

    return browser;
}

QString PixelizePhotoEffect::toString() const
{
    return i18n("Pixelize [PixelSize=") + QString::number(m_pixelSize) + "]";
}
