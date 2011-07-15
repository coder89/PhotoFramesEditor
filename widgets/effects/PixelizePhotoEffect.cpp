#include "PixelizePhotoEffect.h"

#include <QImage>

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
