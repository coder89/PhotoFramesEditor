#include "BlurPhotoEffect.h"
#include "KEditFactory.h"

#include <QtTreePropertyBrowser>
#include <QtIntPropertyManager>
#include <QtSpinBoxFactory>
#include <QTimer>

#include <klocalizedstring.h>

using namespace KIPIPhotoFramesEditor;

class BlurPhotoEffect::BlurUndoCommand : public QUndoCommand
{
        BlurPhotoEffect * m_effect;
        int m_radius;
        int m_opacity;
    public:
        BlurUndoCommand(BlurPhotoEffect * effect, int radius, int opacity);
        virtual void redo();
        virtual void undo();
        void setRadius(int radius);
        void setOpacity(int opacity);
    private:
        void runCommand()
        {
            int temp = m_effect->radius();
            if (temp != m_radius)
            {
                m_effect->setRadius(m_radius);
                m_radius = temp;
            }
            temp = m_effect->opacity();
            if (m_opacity != temp)
            {
                m_effect->setOpacity(m_opacity);
                m_opacity = temp;
            }
        }
};

BlurPhotoEffect::BlurUndoCommand::BlurUndoCommand(BlurPhotoEffect * effect, int radius, int opacity) :
    m_effect(effect),
    m_radius(radius),
    m_opacity(opacity)
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

void BlurPhotoEffect::BlurUndoCommand::setOpacity(int opacity)
{
    m_opacity = opacity;
}

const QString BlurPhotoEffect::RADIUS_STRING = i18n("Radius");
const QString BlurPhotoEffect::OPACITY_STRING = i18n("Opacity");

BlurPhotoEffect::BlurPhotoEffect(int radius, QObject * parent) :
    AbstractPhotoEffect(i18n("Blur effect"), parent),
    m_radius(radius)
{
}

QImage BlurPhotoEffect::apply(const QImage & image)
{
    QImage result = image;
    QPainter p;
    p.begin(&result);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawImage(0,0,AbstractPhotoEffect::apply(blurred(image, image.rect(), m_radius)));
    return result;
}

QtAbstractPropertyBrowser * BlurPhotoEffect::propertyBrowser() const
{
    QtTreePropertyBrowser * browser = new QtTreePropertyBrowser();
    QtIntPropertyManager * intManager = new QtIntPropertyManager(browser);
    KSliderEditFactory * sliderFactory = new KSliderEditFactory(browser);
    browser->setFactoryForManager(intManager,sliderFactory);

    // Radius property
    QtProperty * radius = intManager->addProperty(RADIUS_STRING);
    intManager->setMaximum(radius,200);
    intManager->setMinimum(radius,0);
    browser->addProperty(radius);

    // Opacity property
    QtProperty * opacity = intManager->addProperty(OPACITY_STRING);
    intManager->setMaximum(opacity,100);
    intManager->setMinimum(opacity,0);
    browser->addProperty(opacity);

    intManager->setValue(radius,m_radius);
    intManager->setValue(opacity,m_opacity);
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
    int opacity = m_opacity;

    if (property->propertyName() == RADIUS_STRING)
        radius = manager->value(property);
    else if (property->propertyName() == OPACITY_STRING)
        opacity = manager->value(property);

    sem.acquire();
    if (m_undo_command)
    {
        BlurUndoCommand * undo = dynamic_cast<BlurUndoCommand*>(m_undo_command);
        undo->setRadius(radius);
        undo->setOpacity(opacity);
    }
    else
        m_undo_command = new BlurUndoCommand(this,radius,opacity);
    sem.release();
}
