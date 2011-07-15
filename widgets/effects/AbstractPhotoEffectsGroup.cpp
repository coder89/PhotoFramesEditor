#include "AbstractPhotoEffectsGroup.h"
#include "AbstractPhotoEffect.h"
#include "BlurPhotoEffect.h"
#include "PixelizePhotoEffect.h"

#include "AbstractPhoto.h"

#include <QPainter>

using namespace KIPIPhotoFramesEditor;

AbstractPhotoEffectsGroup::AbstractPhotoEffectsGroup(QObject * parent) :
    QObject(parent)
{
}

void AbstractPhotoEffectsGroup::push_back(AbstractPhotoEffect * effect)
{
    m_effects_list.push_back(effect);
    effect->setParent(this);
    emit effectsChanged();
}

void AbstractPhotoEffectsGroup::push_front(AbstractPhotoEffect * effect)
{
    m_effects_list.push_front(effect);
    effect->setParent(this);
    emit effectsChanged();
}

QPixmap AbstractPhotoEffectsGroup::apply(const QPixmap & pixmap)
{
    QImage image = pixmap.toImage();
    foreach (AbstractPhotoEffect * effect, m_effects_list)
        if (effect)
            image = effect->apply(image);
    return QPixmap::fromImage(image);
}
