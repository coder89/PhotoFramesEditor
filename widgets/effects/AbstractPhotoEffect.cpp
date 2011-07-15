#include "AbstractPhotoEffect.h"
#include "AbstractPhotoEffectsGroup.h"
#include "AbstractPhoto.h"

using namespace KIPIPhotoFramesEditor;

AbstractPhotoEffect::AbstractPhotoEffect(const QString & name, QObject * parent) :
    QObject(parent),
    m_name(name)
{
}

AbstractPhotoEffectsGroup * AbstractPhotoEffect::group() const
{
    return qobject_cast<AbstractPhotoEffectsGroup*>(this->parent());
}

AbstractPhoto * AbstractPhotoEffect::photo() const
{
    AbstractPhotoEffectsGroup * tempGroup = this->group();
    if (tempGroup)
        return tempGroup->photo();
    else
        return 0;
}
