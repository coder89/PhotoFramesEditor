#include "BlurPhotoEffect.h"

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
