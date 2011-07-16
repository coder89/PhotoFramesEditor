#include "BlurPhotoEffect.h"
#include "KEditFactory.h"

#include <QtTreePropertyBrowser>
#include <QtIntPropertyManager>
#include <QtSpinBoxFactory>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

const QString BlurPhotoEffect::RADIUS_STRING = i18n("Radius");

class BlurPhotoEffect::BlurUndoCommand : public QUndoCommand
{
        BlurPhotoEffect * m_effect;
        int m_radius;
    public:
        BlurUndoCommand(BlurPhotoEffect * effect, int radius);
        virtual void redo();
        virtual void undo();
        void setRadius(int radius);
    private:
        void runCommand()
        {
            int temp = m_effect->radius();
            if (temp != m_radius)
            {
                m_effect->setRadius(m_radius);
                m_radius = temp;
            }
        }
};

BlurPhotoEffect::BlurUndoCommand::BlurUndoCommand(BlurPhotoEffect * effect, int radius) :
    m_effect(effect),
    m_radius(radius)
{}

void BlurPhotoEffect::BlurUndoCommand::redo()
{
    runCommand();
}

void BlurPhotoEffect::BlurUndoCommand::undo()
{
    runCommand();
}

void BlurPhotoEffect::BlurUndoCommand::setRadius(int radius)
{
    m_radius = radius;
}

BlurPhotoEffect::BlurPhotoEffect(int radius, QObject * parent) :
    AbstractPhotoEffect(i18n("Blur effect"), parent),
    m_radius(radius)
{
}

QImage BlurPhotoEffect::apply(const QImage & image)
{
    QImage result = image;
    QPainter p(&result);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawImage(0,0,AbstractPhotoEffect::apply(blurred(image, image.rect(), m_radius)));
    return result;
}

QtAbstractPropertyBrowser * BlurPhotoEffect::propertyBrowser() const
{
    QtAbstractPropertyBrowser * browser = AbstractPhotoEffect::propertyBrowser();
    QtIntPropertyManager * intManager = new QtIntPropertyManager(browser);
    KSliderEditFactory * sliderFactory = new KSliderEditFactory(browser);
    browser->setFactoryForManager(intManager,sliderFactory);

    // Radius property
    QtProperty * radius = intManager->addProperty(RADIUS_STRING);
    intManager->setMaximum(radius,200);
    intManager->setMinimum(radius,0);
    browser->addProperty(radius);

    intManager->setValue(radius,m_radius);
    connect(intManager,SIGNAL(propertyChanged(QtProperty*)),this,SLOT(propertyChanged(QtProperty*)));
    connect(sliderFactory,SIGNAL(editingFinished()),this,SLOT(emitEffectChange()));

    return browser;
}

QString BlurPhotoEffect::toString() const
{
    return this->name() + " [" + RADIUS_STRING + "=" + QString::number(m_radius) + "]" ;
}

void BlurPhotoEffect::propertyChanged(QtProperty * property)
{
    QtIntPropertyManager * manager = qobject_cast<QtIntPropertyManager*>(property->propertyManager());
    int radius = m_radius;

    if (property->propertyName() == RADIUS_STRING)
        radius = manager->value(property);
    else
    {
        AbstractPhotoEffect::propertyChanged(property);
        return;
    }

    beginUndoCommandChange();
    if (m_undo_command)
    {
        BlurUndoCommand * undo = dynamic_cast<BlurUndoCommand*>(m_undo_command);
        undo->setRadius(radius);
    }
    else
        m_undo_command = new BlurUndoCommand(this,radius);
    endUndoCommandChange();
}
