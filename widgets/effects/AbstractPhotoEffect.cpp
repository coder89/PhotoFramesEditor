#include "AbstractPhotoEffect.h"
#include "PhotoEffectsGroup.h"
#include "AbstractPhoto.h"
#include "UndoCommandEvent.h"
#include "global.h"

#include <QDebug>

#include <kapplication.h>

using namespace KIPIPhotoFramesEditor;

AbstractPhotoEffect::AbstractPhotoEffect(const QString & name, QObject * parent) :
    QObject(parent),
    m_name(name),
    sem(1),
    m_opacity(100),
    m_undo_command(0)
{
}

PhotoEffectsGroup * AbstractPhotoEffect::group() const
{
    return qobject_cast<PhotoEffectsGroup*>(this->parent());
}

AbstractPhoto * AbstractPhotoEffect::photo() const
{
    PhotoEffectsGroup * tempGroup = this->group();
    if (tempGroup)
        return tempGroup->photo();
    else
        return 0;
}

void AbstractPhotoEffect::emitEffectChange()
{
    sem.acquire();
    if (m_undo_command)
    {
        UndoCommandEvent * event = new UndoCommandEvent();
        event->setUndoCommand(m_undo_command);
        KApplication::postEvent(PhotoFramesEditor::instancePhotoFramesEditor(),event);
        m_undo_command = 0;
    }
    sem.release();
}

QImage AbstractPhotoEffect::apply(const QImage & image)
{
    if (opacity() != 100)
    {
        QImage result(image.size(),QImage::Format_ARGB32_Premultiplied);
        result.fill(Qt::transparent);
        QPainter p;
        p.begin(&result);
        p.drawImage(0,0,image);
        p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        p.fillRect(image.rect(), QColor(0, 0, 0, m_opacity*255/100));
        return result;
    }
    return image;
}
