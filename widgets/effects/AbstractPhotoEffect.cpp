#include "AbstractPhotoEffect.h"
#include "PhotoEffectsGroup.h"
#include "AbstractPhoto.h"
#include "UndoCommandEvent.h"
#include "KEditFactory.h"
#include "global.h"

#include <QtTreePropertyBrowser>
#include <QtIntPropertyManager>

#include <kapplication.h>
#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

class AbstractPhotoEffect::OpacityUndoCommand : public QUndoCommand
{
        AbstractPhotoEffect * m_effect;
        int m_opacity;
    public:
        OpacityUndoCommand(AbstractPhotoEffect * effect, int opacity);
        virtual void redo();
        virtual void undo();
        void setOpacity(int opacity);
    private:
        void runCommand()
        {
            int temp = m_effect->opacity();
            if (m_opacity != temp)
            {
                m_effect->setOpacity(m_opacity);
                m_opacity = temp;
            }
        }
};

AbstractPhotoEffect::OpacityUndoCommand::OpacityUndoCommand(AbstractPhotoEffect * effect, int opacity) :
    m_effect(effect),
    m_opacity(opacity)
{}

void AbstractPhotoEffect::OpacityUndoCommand::redo()
{
    runCommand();
}

void AbstractPhotoEffect::OpacityUndoCommand::undo()
{
    runCommand();
}

void AbstractPhotoEffect::OpacityUndoCommand::setOpacity(int opacity)
{
    m_opacity = opacity;
}

const QString AbstractPhotoEffect::OPACITY_STRING = i18n("Opacity");

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

QImage AbstractPhotoEffect::apply(const QImage & image)
{
    if (opacity() != 100)
    {
        QImage result(image.size(),QImage::Format_ARGB32_Premultiplied);
        QPainter p(&result);
        p.drawImage(0,0,image);
        p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        p.fillRect(image.rect(), QColor(0, 0, 0, m_opacity*255/100));
        return result;
    }
    return image;
}

QtAbstractPropertyBrowser * AbstractPhotoEffect::propertyBrowser() const
{
    QtTreePropertyBrowser * browser = new QtTreePropertyBrowser();
    QtIntPropertyManager * intManager = new QtIntPropertyManager(browser);
    KSliderEditFactory * sliderFactory = new KSliderEditFactory(browser);
    browser->setFactoryForManager(intManager,sliderFactory);

    // Opacity property
    QtProperty * opacity = intManager->addProperty(OPACITY_STRING);
    intManager->setMaximum(opacity,100);
    intManager->setMinimum(opacity,0);
    browser->addProperty(opacity);

    intManager->setValue(opacity,m_opacity);

    connect(sliderFactory,SIGNAL(editingFinished()),this,SLOT(emitEffectChange()));
    connect(intManager,SIGNAL(propertyChanged(QtProperty*)),this,SLOT(propertyChanged(QtProperty*)));

    return browser;
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

void AbstractPhotoEffect::propertyChanged(QtProperty * property)
{
    QtIntPropertyManager * manager = qobject_cast<QtIntPropertyManager*>(property->propertyManager());
    int opacity = m_opacity;

    if (property->propertyName() == OPACITY_STRING)
        opacity = manager->value(property);

    beginUndoCommandChange();
    if (m_undo_command)
    {
        OpacityUndoCommand * undo = dynamic_cast<OpacityUndoCommand*>(m_undo_command);
        undo->setOpacity(opacity);
    }
    else
        m_undo_command = new OpacityUndoCommand(this,opacity);
    endUndoCommandChange();
}
