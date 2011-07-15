#include "AbstractPhotoEffect.h"

using namespace KIPIPhotoFramesEditor;

AbstractPhotoEffect::AbstractPhotoEffect(const QString & name, QObject *parent) :
    QObject(parent),
    m_name(name)
{
}
